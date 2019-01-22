<?php

/**
 * @file
 * Helpers for the admin settings form for this module.
 */

use Drupal\Core\Form\FormStateInterface;

/**
 * Check if the required resouces are enabled.
 *
 * @param array $element
 *   The element to check.
 * @param \Drupal\Core\Form\FormStateInterface $form_state
 *   The Drupal form state.
 * @param array $form
 *   The Drupal form definition.
 */
function islandora_book_admin_settings_form_ingest_derivatives_validate(array $element, FormStateInterface $form_state, array $form) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  // form_error() doesn't seem to work here, it prevents the submit but
  // also prevents the value from being set. So we are using
  // drupal_set_message().
  if (array_key_exists('pdf', $element['#value']) && !islandora_paged_content_can_create_pdf()) {
    $element['#value']['pdf'] = 0;
    $form_state->setValueForElement($element, $element['#value']);
    drupal_set_message(t('ImageMagick must be enabled to derive the PDF datastream.'), 'error');
  }
  if (array_key_exists('image', $element['#value']) && !islandora_paged_content_can_create_images()) {
    $element['#value']['image'] = 0;
    $form_state->setValueForElement($element, $element['#value']);
    drupal_set_message(t('The Large Image Solution Pack must be enabled to derive image datastreams.'), 'error');
  }
  if (array_key_exists('ocr', $element['#value']) && !(\Drupal::moduleHandler()->moduleExists('islandora_ocr'))) {
    $element['#value']['ocr'] = 0;
    $form_state->setValueForElement($element, $element['#value']);
    drupal_set_message(t('The Islandora OCR module must be enabled to derive OCR datastreams.'), 'error');
  }
}
