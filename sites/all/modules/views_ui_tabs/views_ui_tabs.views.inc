<?php

/**
 * @file
 * Defines the View Style Plugins
 */

/**
 * Implements hook_views_plugins().
 */
function views_ui_tabs_views_plugins() {
  $module_path = drupal_get_path('module', 'views_ui_tabs');
  return array(
    'style' => array(
      'ui_tabs' => array(
        'title' => t('UI Tabs'),
        'help' => t('Display the results as tabs.'),
        'handler' => 'views_ui_tabs_plugin_style_tabs',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'path' => $module_path,
        'theme' => 'views_ui_tabs_tabs',
        'theme path' => $module_path . '/theme',
        'theme file' => 'views_ui_tabs.theme.inc',
      ),
    ),
  );
}
