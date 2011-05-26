<?php 
function sci_aggregator_block_item($item, $feed = 0) {
  global $user;

  $output = '';
  if ($user->uid && module_exists('post') && user_access('create post entries')) {
    if ($image = theme('image', 'misc/blog.png', t('post it'), t('post it'))) {
      $output .= '<div class="icon">'. l($image, 'node/add/post', array('attributes' => array('title' => t('Comment on this news item in your personal postings.'), 'class' => 'post-it'), 'query' => "iid=$item->iid", 'html' => TRUE)) .'</div>';
    }
  }

  // Display the external link to the item.
  $output .= '<a href="'. check_url($item->link) .'">'. check_plain($item->title) ."</a>\n";

  return $output;
}

/**
 * Generate the HTML output for a single local task link.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_task($link, $active = FALSE) {
  return '<li '. ($active ? 'class="active " ' : '') .$secondary'>'. $link ."</li>\n";
}
?>