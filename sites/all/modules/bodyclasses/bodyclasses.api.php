<?php

/**
 * @file
 * API documentation for the Body Classes Drupal module.
 */

/**
 * Exposes different methods to obtain classes for the body tag.
 *
 * @return array An associative array, keyed by machine name, where the "name"
 *   represents a human readable name for the method, and "function" is a
 *   callable function to retrieve the related classes.
 *
 * @see bodyclasses_bodyclasses_info()
 */
function hook_bodyclasses_info() {
  // Tell the Body Classes module that we can add classes about the Node to
  // the body tag.
  if (module_exists('node')) {
    $bodyclasses['node'] = array(
      // The human readable name.
      'name' => t('Node'),
      // The function that will return an array of all related classes.
      'function' => 'mymodule_bodyclasses_node',
    );
  }

  return $bodyclasses;
}
