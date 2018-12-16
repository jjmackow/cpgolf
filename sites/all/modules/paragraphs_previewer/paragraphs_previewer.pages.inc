<?php
/**
 * @file
 * Page callbacks for paragraphs_previewer.
 */

/**
 * Render a preview of the paragraph on an entity edit form.
 *
 * @return mixed
 *   A renderable array, text markup, or MENU constant.
 */
function paragraphs_previewer_form_preview() {
  // Form is required.
  if (empty($_GET['form'])) {
    return MENU_ACCESS_DENIED;
  }

  // Validate token.
  if (empty($_GET['token']) || !drupal_valid_token($_GET['token'], 'paragraphs_previewer_form_preview')) {
    return MENU_ACCESS_DENIED;
  }

  // Check minimum arguments required.
  $item_parents = func_get_args();
  if (empty($item_parents) || count($item_parents) < 3) {
    return t('No preview available.');
  }

  // Get the form cache.
  $form_build_id = $_GET['form'];
  $form_state = form_state_defaults();
  $form = form_get_cache($form_build_id, $form_state);
  if (empty($form)) {
    return t('No preview available.');
  }

  $parents = $item_parents;
  $delta = array_pop($item_parents);
  $langcode = array_pop($item_parents);
  $field_name = array_pop($item_parents);
  $field_parents = $item_parents;

  $element = drupal_array_get_nested_value($form, $parents);
  if (isset($element['#access']) && empty($element['#access'])) {
    return t('No preview available.');
  }

  $field_state = field_form_get_state($field_parents, $field_name, $langcode, $form_state);
  if (empty($field_state['instance']) || empty($field_state['entity'][$delta])) {
    return t('No preview available.');
  }

  // Clone the entity so that the field state is not corrupted.
  $entity = clone($field_state['entity'][$delta]);
  $entity_type = 'paragraphs_item';
  $entity_bundle = $entity->bundle();

  // Get / set the host entity.
  $host_entity = NULL;
  $host_entity_type = NULL;
  $host_entity_ref = $entity->hostEntity();
  if (!empty($host_entity_ref)) {
    $host_entity = clone($host_entity_ref);
    $host_entity_type = $entity->hostEntityType();
  }
  elseif (!empty($element['#host_entity_type']) && !empty($element['#host_entity'])) {
    // Requires: https://www.drupal.org/node/2564327#comment-11102791
    $host_entity = clone($element['#host_entity']);
    $host_entity_type = $element['#host_entity_type'];
    $entity->setHostEntity($element['#host_entity_type'], $host_entity, $langcode, FALSE);
  }

  if (empty($host_entity)) {
    return t('No preview available.');
  }

  // Check view access if there is a host entity.
  if (!empty($host_entity) && !entity_access('view', $entity_type, $entity)) {
    return t('No preview available.');
  }

  // Ensure fields are loaded.
  // Note: This is needed for previews of new and changes nested entities.
  // @see https://www.drupal.org/node/1447338
  // @see https://www.drupal.org/node/2707463
  $child_field_instances = field_info_instances($entity_type, $entity_bundle);
  foreach ($child_field_instances as $child_field_name => $child_field_instance) {
    $child_items = field_get_items($entity_type, $entity, $child_field_name, $langcode);
    if ($child_items) {
      foreach ($child_items as &$child_item) {
        if (isset($child_item['entity']) && !empty($child_item['entity']) && method_exists($child_item['entity'], 'entityType') && empty($child_item['entity']->_paragraphs_previewer_fields_loaded)) {
          $child_entity_type = $child_item['entity']->entityType();
          list($child_id, $child_revision_id, $child_bundle) = entity_extract_ids($child_entity_type, $child_item['entity']);
          _field_invoke_multiple('load', $child_entity_type, array($child_id => $child_item['entity']));
          $child_item['entity']->_paragraphs_previewer_fields_loaded = TRUE;
        }
      }
      unset($child_item);
    }
    $entity->{$child_field_name}[$langcode] = $child_items;
  }

  // Set a flag so others can do special field preparations.
  $entity->_paragraphs_previewer_rendering = TRUE;
  $host_entity->_paragraphs_previewer_rendering = TRUE;

  // Build the field item with the form based entity.
  $field_item = array(
    'value' => !empty($entity->item_id) ? $entity->item_id : NULL,
    'revision_id' => !empty($entity->revision_id) ? $entity->revision_id : NULL,
    'entity' => $entity,
  );

  // Render the item.
  // Note: Using field_view_field() avoid issue with field_view_value()
  // not finding the correct language due to field_language() static cache
  // indexed by entity type and id which fails for multiple new entities of
  // the same entity type that do not have an id.
  $host_entity->{$field_name}[$langcode] = array($field_item);
  $previewer_view_mode = variable_get('paragraphs_previewer_view_mode', 'full');
  $item_render = field_view_field($host_entity_type, $host_entity, $field_name, $previewer_view_mode, $langcode);

  // Allow others to alter the output, add CSS, add JS based on the paragaph
  // and node object.
  $node = NULL;
  if (isset($form_state['node'])) {
    $node = clone($form_state['node']);
  }

  $context = array(
    'paragraph_item' => $entity,
    'element' => $element,
    'node' => $node,
    'form' => $form,
    'form_state' => $form_state,
  );
  $build = array('item' => $item_render);
  drupal_alter('paragraphs_previewer_page', $build, $context);

  $empty_check = array_diff_key($item_render, array('#access' => 1));
  return $empty_check ? $build : t('No preview available.');
}
