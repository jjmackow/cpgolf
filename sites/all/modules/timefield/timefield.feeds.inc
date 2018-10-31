<?php

/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets()
 */
function timefield_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'timefield') {
      $targets[$name . ':start'] = array(
        'name' => t('@name: Start', array('@name' => $instance['label'])),
        'callback' => 'timefield_feeds_set_target',
        'description' => t('The start time for the @name field. Also use if mapping both start and end.', array('@name' => $instance['label'])),
        'real_target' => $name,
      );
      $targets[$name . ':end'] = array(
        'name' => t('@name: End', array('@name' => $instance['label'])),
        'callback' => 'timefield_feeds_set_target',
        'description' => t('The end time for the @name field.', array('@name' => $instance['label'])),
        'real_target' => $name,
      );
    }
  }
}

/**
 * Callback for timefield_feeds_processor_targets_alter().
 *
 * @param object $source
 *   Field mapper source settings.
 * @param object $entity
 *   An entity object, for instance a node object.
 * @param string $target
 *   A string identifying the target on the node.
 * @param string $element_value
 *   The value to populate the target with.
 * @param array $mapping
 *   Associative array of the mapping settings from the per mapping
 *   configuration form.
 */
function timefield_feeds_set_target($source, $entity, $target, $element_value, $mapping) {
  if (empty($element_value)) {
    return;
  }

  // Handle non-multiple value fields.
  if (!is_array($element_value)) {
    $element_value = array($element_value);
  }

  // Get the timefield field name and sub_field (start / end).
  list($field_name, $sub_field) = explode(':', $target);

  // If the field is already set on the given entity, update the existing value
  // array, otherwise, start with a fresh field value array.
  $field = isset($entity->$field_name) ? $entity->$field_name : array();

  // Loop over the field values array.
  foreach ($element_value as $delta => $value) {
    // Make sure we have a valid time before trying to set the value on the field.
    $date_value = date_parse($value);
    if (empty($date_value['error_count'])) {
      $parsed_value = timefield_time_to_integer($value);
      if ($parsed_value) {
        $sub_field = ($sub_field == 'start') ? 'value' : 'value2';
        $language = isset($mapping['language']) ? $mapping['language'] : LANGUAGE_NONE;
        $field[$language][$delta][$sub_field] = $parsed_value;
      }
    }
  }

  // Add the field to the entity definition.
  $entity->{$field_name} = $field;
}
