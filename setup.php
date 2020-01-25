<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

require __DIR__ . '/vendor/autoload.php';
use Automattic\WooCommerce\Client;


$site='http://192.168.31.62/';//your store url
$key='ck_4afdc97e7f607e68af52393fced997b7972427ff'; //your consumer key
$secret='cs_f238d1472aaeed114b639c02aa3f4df93bb66433'; //your consumer secret
$woocommerce = new Client(
   
    $site,
    $key,
    $secret,
    [
        'wp_api' => true, // Enable the WP REST API integration
        'version' => 'wc/v2',  // WooCommerce WP REST API version
        'verify_ssl' => false

    ]
);


 
 
 ?>
