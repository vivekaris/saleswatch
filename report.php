<?php

use Automattic\WooCommerce\HttpClient\HttpClientException;

include './setup.php';

// Takes raw data from the request
$json = file_get_contents('php://input');
// Converts it into a PHP object
$demo = json_decode($json);

$query = [
		'period' => 'month'
	];

if ($demo->find == 'month') {
	$query = [
		'period' => 'month'
	];
}
if ($demo->find == 'today') {
	$query = [
		'date_min' => date('Y-m-d'),
		'date_max' => date('Y-m-d')
             ];
}


$result = $woocommerce->get('reports/sales', $query);

foreach ($result as $sales) {
	$data['net_sales'] = $sales->net_sales;
	$data['total_orders'] = $sales->total_orders;
	if ($sales->total_orders > 2) {
		$data['message'] = "New Order Recived Process it from Dashboard: report by {$demo->find}";
	} else {
		$data['message'] = " U hv {$sales->total_orders} order. report by {$demo->find}";
	}
}

echo json_encode($data);
