/**
 * Add a comment to a review
 */
action swarm_add_review_comment
{
  set loginresult = restful_post($server, 80, '/login', {
  	'user' => 'Robert',
  	'password' => 'perforce'
  }, 'cookiejar');
  
  // DEBUG
  echo "isValid = ${loginresult.isValid}";
  echo "error = ${loginresult.error}";
  echo "info.userName = ${loginresult.info.userName}";
  // END-DEBUG
  
  restful_post(server: $server, uri: '/comments/add', cookiesfrom: $cookiejar, params: {
  	'topic' => "reviews/${review:-15}",
  	'context' => "{\"reviews\":${review:-15}}",
  	'user' => 'Robert',
  	'body' => $body
  });
}


/**
 * Get a list of reviews from the server
 */
function swarm_get_reviews(server)
{
  set loginresult = restful_post($server, 80, '/login', {
  	'user' => 'Robert',
  	'password' => 'perforce'
  }, 'cookiejar');
  
  set reviewresult = restful_post($server, 80, '/reviews?format=json', $cookiejar);
  
  // DEBUG
  for(n = 0; $n < ${reviewresult.reviews.length()}; n = $n + 1) {
    set review = ${reviewresult.reviews[$n]};
    set desc = regsub(${review.description}, @'<span class="first-line">(.*)<\\/span>', '\1');
    echo "${review.id} - $desc";
  }
  // END-DEBUG
  
  return $reviewresult;
}

//action p4_PostAction
//{
//  swarm_add_review_comment(server: 'swarm', review: $url_review, body: 'Comment posted by DM2');
//}
