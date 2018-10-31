<?php

/**
 * Alter the display settings.
 *
 * @param $display
 * @param $entity
 * @param $entity_type
 */
function hook_datetime_formatter_settings_alter(&$display, &$entity, $entity_type) {
  // Use local time setting for US (only).
  if ($entity_type === 'page' && $entity->field_location[LANGUAGE_NONE][0]['country'] === 'US') {
    $display['settings']['local_time'] = 1;
  }
}
