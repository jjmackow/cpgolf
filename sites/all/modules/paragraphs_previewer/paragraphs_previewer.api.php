<?php
/**
 * @file
 * Hooks provided by the paragraphs_previewer module.
 */

/**
 * Alter the paragraphs previewer page.
 *
 * This is provides a hook to alter the page with context of the paragraph item,
 * the parent node and form.
 *
 * Example use case: Add custom CSS and JS to the preview page based on field
 * values of the node.
 *
 * @param array $render
 *   The render array for the paragraph.
 * @param array $context
 *   An array with the following:
 *   - 'paragraph_item' object: The paragraph item object.
 *   - 'element' array: The element for the paragraph item. Only present on
 *      form previews.
 *   - 'node' object: The parent node object.
 *   - 'form' array: The parent form. Only present on form previews.
 *   - 'form_state' array: The parent form state. Only present on form previews.
 */
function hook_paragraphs_previewer_page_alter(&$render, $context) {
  if (isset($context['node']) && !empty($context['node']->field_my_custom[LANGUAGE_NONE][0]['value'])) {
    $render['#attached']['css'][] = drupal_get_path('module', 'my_custom') . '/my_custom.css';
  }
}
