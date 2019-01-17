<?php

/**
 * @file
 * Theme hooks.
 */

use Drupal\Core\Url;

use Drupal\islandora\Authentication\Provider\TokenAuth;

/**
 * Prepares variables for islandora_book_book templates.
 *
 * Default template: islandora-book-book.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_book_book(array &$variables) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora', 'includes/datastream');

  $object = $variables['object'];

  $variables['viewer_id'] = islandora_get_viewer_id('islandora_book_viewers');
  $variables['viewer_params'] = [
    'object' => $object,
    'pages' => islandora_paged_content_get_pages($object),
    'page_progression' => islandora_paged_content_get_page_progression($object),
  ];

  $variables['display_metadata'] = \Drupal::config('islandora_book.settings')->get('islandora_book_metadata_display');
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);
  if ($variables['viewer_id'] && $variables['viewer_id'] != 'none') {
    $viewer_callback = islandora_get_viewer_callback($variables['viewer_id']);
    if (function_exists($viewer_callback)) {
      $viewer = $viewer_callback($variables['viewer_params'], $variables['object']);
      if ($viewer) {
        $variables['viewer'] = $viewer;
      }
    }
  }
}

/**
 * Prepares variables for islandora_book_page templates.
 *
 * Default template: islandora-book-page.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_book_page(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $object = $variables['object'];
  $results = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOf');
  $result = reset($results);
  $variables['book_object_id'] = $result ? $result['object']['value'] : FALSE;

  $params = [];
  if (isset($object['JP2']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JP2'])) {
    // Get token to allow access to XACML protected datastreams.
    // Always use token authentication in case there is a global policy.
    $token = TokenAuth::getObjectToken($object->id, 'JP2', 2);
    $params['token'] = $token;
    $params['pid'] = $object->id;
    $params['dsid'] = 'JP2';
  }
  if (isset($object['JPG']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JPG'])) {
    $params['backup_image'] = [
      '#theme' => 'image',
      '#uri' => Url::fromRoute('islandora.view_datastream_view', ['object' => $object->id, 'datastream' => 'JPG'])->toString(),
    ];
  }

  $variables['viewer_id'] = islandora_get_viewer_id('islandora_book_page_viewers');
  $variables['viewer_params'] = $params;
  if ($variables['viewer_id'] && $variables['viewer_id'] != 'none') {
    $viewer_callback = islandora_get_viewer_callback($variables['viewer_id']);
    if (function_exists($viewer_callback)) {
      $viewer = $viewer_callback($variables['viewer_params'], $variables['object']);
      if ($viewer) {
        $variables['viewer'] = $viewer;
      }
    }
  }
}
