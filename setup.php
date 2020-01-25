<?php require __DIR__ . '/vendor/autoload.php';
use Automattic\WooCommerce\Client;


$site='';//your store url
$key=''; //your consumer key
$secret=''; //your consumer secret
$woocommerce = new Client(
    $site,$key,$secret,
    [
        'wp_api' => true,
        'version' => 'wc/v2'
    ]
);
