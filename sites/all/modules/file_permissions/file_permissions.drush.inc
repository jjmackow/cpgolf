<?php
/**
 * @file
 * Provide Drush integration for File permissions module.
 */

define('DS', DIRECTORY_SEPARATOR);

/**
 * Implements hook_drush_help().
 */
function file_permissions_drush_help($section) {
  switch ($section) {
    case 'drush:file-permissions':
      return dt('Rebuild your local environment files permissions.');
  }
}

/**
 * Implements hook_drush_command().
 */
function file_permissions_drush_command() {
  $items = array();

  $items['file-permissions'] = array(
    'description' => 'Rebuild your local environment files permissions.',
    'callback' => 'drush_file_permissions',
    // Need FULL bootstrap level to be able to use variable_get().
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('fp'),
  );

  return $items;
}

/**
 * Set file permissions.
 */
function drush_file_permissions() {
  ini_set('memory_limit', -1);
  // Before calling this drush needs to be bootstrapped to
  // DRUPAL_BOOTSTRAP_DATABASE.
  if (!drush_bootstrap_to_phase(DRUSH_BOOTSTRAP_DRUPAL_DATABASE)) {
    return drush_set_error('DRUPAL_SITE_NOT_FOUND', dt('You need to specify an alias or run this command within a Drupal site.'));
  }

  // Define all files to be included.
  $includes = array(
    DRUPAL_ROOT . DS . 'includes' . DS . 'bootstrap.inc',
    DRUPAL_ROOT . DS . 'includes' . DS . 'module.inc',
    DRUPAL_ROOT . DS . 'includes' . DS . 'file.inc',
  );

  // Load all needed files.
  foreach ($includes as $include) {
    if (file_exists($include)) {
      require_once $include;
    }
  }

  // Define default paths.
  $default = 'sites' . DS . 'default';
  $private = 'sites' . DS . 'default' . DS . 'private';
  $files = 'sites' . DS . 'default' . DS . 'files';
  // Get paths from variables. Not using drush_variable_get here as it throws
  // an error when variable is empty.
  $private_variable = variable_get('file_private_path', $private);
  $files_variable = variable_get('file_public_path', $files);
  $temp_variable = variable_get('file_temporary_path', '/tmp');

  // Sometimes sites/default subdirectory is not set to 755, ensure it is.
  chmod($default, 0755);

  // Create sites/default/files subdirectory if it doesn't exist.
  $files_dir = DRUPAL_ROOT . DS . $files;
  if (!file_exists($files_dir)) {
    $mkdir = drush_mkdir($files_dir, TRUE);
    if (!$mkdir) {
      drush_log(dt('Attempted to create non existent @dir directory. Check `drush vget file_public_path`', array(
        '@dir' => $private_dir,
      )), 'warning');
      exit;
    }
  }

  // Detect sites/default/private subdirectory.
  $private_dir = DRUPAL_ROOT . DS . $private;
  if (!empty($private_variable)) {
    // If directory is relative (not starting with /), lets create it in
    // Drupal docroot.
    if (drush_is_absolute_path($private_variable)) {
      $private_dir = $private_variable;
    }
    else {
      $private_dir = DRUPAL_ROOT . DS . $private_variable;
    }
  }
  // Create sites/default/private subdirectory if it doesn't exist.
  if (!file_exists($private_dir)) {
    $mkdir = drush_mkdir($private_dir, TRUE);
    if (!$mkdir) {
      drush_log(dt('Attempted to create non existent @dir directory. Check `drush vget file_private_path`', array(
        '@dir' => $private_dir,
      )), 'warning');
      exit;
    }
  }

  // Get apache user.
  $user = drush_shell_exec('ps axho user,group,comm|grep -E "httpd|apache|nginx"|uniq|grep -v "root"|awk \'END {if ($1 && $2) print $1}\'');
  $user = current(drush_shell_exec_output());
  drush_log(dt('Your httpd user: ' . $user), 'status');

  // Get first group of apache user.
  $groups = drush_shell_exec('id -Gn ' . $user);
  $groups = current(drush_shell_exec_output());
  $groups = explode(' ', $groups);
  $group = current($groups);
  drush_log(dt('Your httpd group: ' . $group), 'status');

  drush_log(dt('You will be prompted for admin password to be able to change permissions with `sudo` command.'), 'warning');
  // Change permissions properly to files and private subdirectories.
  drush_shell_exec('nohup sudo chown -Rv ' . $user . ':' . $group . ' ' . $files_dir . '  > /dev/null');
  drush_shell_exec('nohup sudo chown -Rv ' . $user . ':' . $group . ' ' . $private_dir . ' > /dev/null');
  drush_shell_exec('nohup sudo chgrp -Rv ' . $group . ' ' . $files_dir . '  > /dev/null');
  drush_shell_exec('nohup sudo chgrp -Rv ' . $group . ' ' . $private_dir . ' > /dev/null');
  // The 2 means that the group id will be preserved for any new files created
  // in this directory. What that means is that `www-data` will always be the
  // group on any files, thereby ensuring that web server and the user will both
  // always have write permissions to any new files that are placed in this
  // directory.
  drush_shell_exec('nohup sudo chmod -R 2775 ' . $files_dir . '  > /dev/null');
  drush_shell_exec('nohup sudo chmod -R 2775 ' . $private_dir . ' > /dev/null');
  drush_shell_exec('nohup sudo chmod -R g+w ' . $files_dir . '  > /dev/null');
  drush_shell_exec('nohup sudo chmod -R g+w ' . $private_dir . ' > /dev/null');
  // Change settings.php permissions to 444 as per:
  // https://www.drupal.org/documentation/install/settings-file
  drush_shell_exec('nohup chmod 444 ' . $default . DS . 'settings.php  > /dev/null');

  // Create .htaccess files in public, private and temp subdirectories.
  file_ensure_htaccess();
}
