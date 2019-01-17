<?php

/**
 * @file
 * Contains two local actions for ingesting pages.
 */

/**
 * Local menu action to present an ingest page form.
 *
 * @param AbstractObject $object
 *   The book to ingest into.
 *
 * @return string
 *   The HTML representation of the ingest page form.
 */
function islandora_book_ingest_page(AbstractObject $object) {
  $user = \Drupal::currentUser();
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/ingest.form');
  $tuque = islandora_get_tuque_connection();
  $page = $tuque->repository->constructObject(islandora_get_namespace($object->id));
  $page->owner = $user->getUsername();
  $page->label = 'New Page';
  $page->models = 'islandora:pageCModel';
  // @FIXME
// drupal_set_title() has been removed. There are now a few ways to set the title
// dynamically, depending on the situation.
//
//
// @see https://www.drupal.org/node/2067859
// drupal_set_title(t('Add page to @book', array('@book' => $object->label)));

  return \Drupal::formBuilder()->getForm('Drupal\islandora\Form\IngestForm', [
    'book' => $object,
    'models' => ['islandora:pageCModel'],
    'objects' => [$page],
    'parent' => $object->id,
  ]);
}

/**
 * Creates page objects and associates them with book object.
 *
 * @param Repository $repository
 *   Active repository object to build NewFedoraObjecti.
 * @param array $config
 *   Associative array of required values.
 * @param string $pages_directory
 *   This is a hack.  The finished callback needs this value so it can delete
 *   the temporary directory used to store book pages.
 */
function islandora_book_add_pages(Repository $repository, array $config, $pages_directory, &$context) {
  module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $context['results']['pages_directory'] = $pages_directory;

  // Create object.
  $object = $repository->constructObject($config['namespace']);
  $object->label = pathinfo($config['image'], PATHINFO_FILENAME);
  $ds_label = pathinfo($config['image'], PATHINFO_BASENAME);
  islandora_paged_content_update_datastream($object, $config['image'], 'OBJ', $ds_label, NULL, 'M', FALSE);
  $rels_ext = $object->relationships;
  $parent = $config['book_pid'];
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:pageCModel');
  if (\Drupal::moduleHandler()->moduleExists('islandora_ocr')) {
    module_load_include('inc', 'islandora_ocr', 'includes/derivatives');
    islandora_ocr_set_generating_rels_ext_statements($object, !$config['ignored_derivs']['ocr'], !$config['ignored_derivs']['hocr']);
  }
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $parent);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $config['page_number'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $config['page_number'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
  islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $parent);
  islandora_add_object($object);
}

/**
 * Alters page and sequence number of page object to allow for inserted pages.
 *
 * @param array $page_to_reindex
 *   Array Containing page pid and current page number.
 * @param int $offset
 *   Offset to determine new page number.
 */
function islandora_book_insert_sequence_gap(array $page_to_reindex, $offset) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $new_page_num = (int) $page_to_reindex['page'] + $offset;
  $page_object = islandora_object_load($page_to_reindex['pid']);
  $rels_ext = $page_object->relationships;
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $new_page_num, TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $new_page_num, TRUE);
}
