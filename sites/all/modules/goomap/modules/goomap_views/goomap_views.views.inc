<?php

/**
 * @file
 * File for Goomp views integration.
 */

/**
 * Define leaflet views style.
 *
 * Implements hook_views_plugins().
 */
function goomap_views_views_plugins() {
  $plugins = array(
    'module' => 'goomap',
    'style' => array(
      'goomap' => array(
        'title' => t('Goo Map'),
        'path' => drupal_get_path('module', 'goomap_views'),
        'handler' => 'goomap_views_plugin_style',
        'uses fields' => TRUE,
        'uses row plugin' => FALSE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );
  return $plugins;
}
