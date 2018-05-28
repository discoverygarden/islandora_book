<?php

namespace Drupal\islandora_book\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Module settings form.
 */
class Admin extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'islandora_book_admin_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('islandora_book.settings');

    $config->set('islandora_book_ingest_derivatives', $form_state->getValue('islandora_book_ingest_derivatives'));
    $config->set('islandora_book_parent_book_solr_field', $form_state->getValue('islandora_book_parent_book_solr_field'));
    $config->set('islandora_book_metadata_display', $form_state->getValue('islandora_book_metadata_display'));

    $config->save();
    islandora_set_viewer_info('islandora_book_viewers', $form_state->getValue('islandora_book_viewers'));
    islandora_set_viewer_info('islandora_book_page_viewers', $form_state->getValue('islandora_book_page_viewers'));
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['islandora_book.settings'];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $form_state->loadInclude('islandora_book', 'inc', 'includes/admin.form');
    $form_state->loadInclude('islandora', 'inc', 'includes/solution_packs');
    $get_default_value = function ($name) use ($form_state) {
      $values = $form_state->getValues();
      return isset($values[$name]) ? $values[$name] : $this->config('islandora_book.settings')->get($name);
    };
    $form = [
      'local_derivative_settings' => [
        '#type' => 'fieldset',
        '#title' => $this->t('Create Page/Book Derivatives Locally'),
        '#description' => $this->t('These options allow you to create derivatives
          automatically when ingesting pages or issues, using this server. If you
          intend to only use microservices to generate derivatives, you should
          not have any of these settings enabled.'),
        'islandora_book_ingest_derivatives' => [
          '#type' => 'checkboxes',
          '#element_validate' => ['islandora_book_admin_settings_form_ingest_derivatives_validate'],
          '#options' => [
            'pdf' => $this->t('PDF datastream. <b>Requires</b> <i>ImageMagick</i>'),
            'image' => $this->t('Image datastreams (TN, JPEG, JP2). <b>Requires</b> <i>Large Image Solution Pack</i>'),
            'ocr' => $this->t('OCR datastreams (OCR, HOCR). <b>Requires</b> <i>Islandora OCR module</i>'),
          ],
          '#default_value' => $get_default_value('islandora_book_ingest_derivatives'),
        ],
      ],
    ];

    // Solr field containing the parent book PID.
    $form['islandora_book_parent_book_solr_field'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Parent Solr Field'),
      '#description' => $this->t("Solr field containing the parent book's PID."),
      '#default_value' => $this->config('islandora_book.settings')->get('islandora_book_parent_book_solr_field'),
      '#size' => 30,
    ];

    // Metadata display.
    $form['metadata_display_fieldset'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Metadata display'),
    ];

    $form['metadata_display_fieldset']['islandora_book_metadata_display'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Display object metadata'),
      '#description' => $this->t('Display object metadata below object viewer.'),
      '#default_value' => $this->config('islandora_book.settings')->get('islandora_book_metadata_display'),
    ];

    $form += islandora_viewers_form('islandora_book_viewers', NULL, 'islandora:bookCModel');
    $form['book_viewers'] = $form['viewers'];
    $form['book_viewers']['#title'] = $this->t('Book Viewers');
    unset($form['viewers']);
    $form += islandora_viewers_form('islandora_book_page_viewers', 'image/jp2', 'islandora:pageCModel');
    $form['page_viewers'] = $form['viewers'];
    $form['page_viewers']['#title'] = $this->t('Page Viewers');
    unset($form['viewers']);
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Submit'),
    ];
    return $form;
  }

}
