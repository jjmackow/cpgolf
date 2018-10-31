<?php
function ds_pdf_open_brochure() {
  return array(
    'label' => t('Big open brochure for PDF printing'),
    'regions' => array(
      'first' => t('First'),


    ),
    // Add this line if there is a default css file.
    'css' => TRUE,
    // Add this line if you're using DS 2.x for icon preview
    'image' => TRUE,
  );
}