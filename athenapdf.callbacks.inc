<?php

/**
 * Callback for menu: node/%node/pdf
 * Redirects to a PDF file for the given node, generating the file if required.
 *
 * @param $node
 *   The currently processed node object.
 */
function athenapdf_view_node_page($node) {
  // Display the pdf file if already exists.
  $fid = athenapdf_get_cached_file($node->nid);

  if (empty($fid)) {
    // Try to generate the PDF file for the given node.
    $file = athenapdf_create_for_node($node);
  }
  else {
    $file = file_load($fid);
  }

  // Report an error if we could not generate the PDF files.
  if (!$file) {
    watchdog(
      'athenapdf',
      'Could not generate pdf for node: @nid',
      ['@nid' => $node->nid],
      WATCHDOG_ERROR
    );
    drupal_set_message(
      t(
        "We're sorry but the PDF for the current page is not available, please try again later"
      ),
      'error',
      FALSE
    );
    drupal_goto('node/' . $node->nid);
  }

  $url = file_create_url($file->uri);
  header('Location: ' . $url . '', TRUE, 302);
  drupal_exit($url);
}
