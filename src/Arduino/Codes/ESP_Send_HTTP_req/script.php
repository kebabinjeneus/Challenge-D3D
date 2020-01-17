<?php
// Your ID and token
$blogID = '8070105920543249955';
$authToken = 'OAuth 2.0 token here';

// The data to send to the API
$postData = array(
    'kind' => 'blogger#post',
    'blog' => array('id' => $blogID),
    'title' => 'A new post',
    'content' => 'With <b>exciting</b> content...'
);

// Create the context for the request
$context = stream_context_create(array(
    'http' => array(
        // http://www.php.net/manual/en/context.http.php
        'method' => 'POST',
        'header' => "Authorization: {$authToken}\r\n".
            "Content-Type: application/json\r\n",
        'content' => json_encode($postData)
    )
));

// Send the request
$response = file_get_contents('https://www.googleapis.com/blogger/v3/blogs/'.$blogID.'/posts/', FALSE, $context);

// Check for errors
if($response === FALSE){
    die('Error');
}

// Decode the response
$responseData = json_decode($response, TRUE);

// Print the date from the response
echo $responseData['published'];