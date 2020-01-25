<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

use Automattic\WooCommerce\HttpClient\HttpClientException;

include './setup.php';

// Takes raw data from the request


$query = [
    'period' => 'month'
                 ];

                 try {

$result = $woocommerce->get('reports/sales', $query);

foreach ($result as $sales) {
	$data['net_sales'] = $sales->net_sales;
	$data['total_orders'] = $sales->total_orders;
	if ($sales->total_orders > 2) {
		$data['message'] = "New Order Recived Process it from Dashboard";
	} else {
		$data['message'] = " U hv {$sales->total_orders} order in month.";
	}
}

echo json_encode($data);
} catch (HttpClientException $e) {
    echo '<pre><code>' . print_r( $e->getMessage(), true ) . '</code><pre>'; // Error message.
    echo '<pre><code>' . print_r( $e->getRequest(), true ) . '</code><pre>'; // Last request data.
    echo '<pre><code>' . print_r( $e->getResponse(), true ) . '</code><pre>'; // Last response data.
}

