<?php

/**
 * @file
 * Views integration for the views_unformatted_id module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_unformatted_id_views_plugins() {
  $plugins = array();

  $plugins['style']['unformatted_id'] = array(
    'title' => t('Unformatted list with CSS row ids'),
    'help' => t('Displays rows one after another with unique CSS row ids.'),
    'handler' => 'views_plugin_style_unformatted_id',
    'parent' => 'views_plugin_display_default',
    'theme' => 'views_view_unformatted_id',
    'uses row plugin' => TRUE,
    'uses row class' => TRUE,
    'uses grouping' => TRUE,
    'uses options' => TRUE,
    'uses fields' => TRUE,
    'type' => 'normal',
    'help topic' => 'style-unformatted-id',
  );

  return $plugins;
}
