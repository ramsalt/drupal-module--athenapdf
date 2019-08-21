<?php

/**
 * Process the node export.
 *
 * @param $node
 *   Node object.
 * @param string $output_type
 *   Defines the output type for FPDF class. Default: send to browser as file
 *   stream.
 *
 * @return string
 */
function athenapdf_process_node($node, $output_type = 'I') {
  return athenapdf_process_node_athenapdf($node, $output_type);
}

/**
 * Sends a request for the node to a AthenaPDF endpoint and the PDF.
 *
 * @param $node
 *   Node object.
 * @param string $output_type
 *   Defines the output type for FPDF class. Default: send to browser as file
 *   stream.
 *
 * @return mixed
 */
function athenapdf_process_node_athenapdf($node, $output_type) {
  global $base_url;

  // TODO Athenapdf Front Domain variable that can be configurable. If not set used the base URL
  // Have a form in the config page. Call field names as the variables
  // https://api.drupal.org/api/drupal/modules%21forum%21forum.admin.inc/function/forum_admin_settings/7.x



  $node_url = $base_url . base_path() . 'node/' . $node->nid;
  $athenapdf_endpoint = variable_get('athenapdf_endpoint');
  $athenapdf_secret = variable_get('athenapdf_secret');

  if (empty($athenapdf_endpoint)) {
    throw new InvalidArgumentException('You must define a "athenapdf_endpoint".');
  }

  if (empty($athenapdf_secret)) {
    throw new InvalidArgumentException('You must define a "athenapdf_secret".');
  }

  $data = array(
    'auth' => $athenapdf_secret,
    'url' => $node_url . '/pdf/html?athenapdf_token=' . $athenapdf_secret . '&c=' . microtime(),
  );

  $options = array(
    'method' => 'GET',
    'timeout' => 30,
  );

  $response = drupal_http_request($athenapdf_endpoint . '?' . drupal_http_build_query($data), $options);
  if ($response->code != 200) {
    throw new RuntimeException('Error: ' . $response->code . '. Data: ' . $response->data);
  }

  // $output_type == 'I' renders the PDF inline in the browser.
  // Otherwise return it as a string.
  if ($output_type === 'I') {
    header('Content-type: application/pdf');
    header('Content-Disposition: inline; filename="pdf-export-' . $node->nid . '.pdf"');
    header('Content-Transfer-Encoding: binary');
    header('Accept-Ranges: bytes');

    print $response->data;
    drupal_exit();
  }

  return $response->data;
}

