<?php
// $Id: simplenews-status.tpl.php,v 1.4 2009/03/14 02:01:38 sutharsan Exp $

/**
 * @file
 * Default theme implementation to display the simplenews newsletter status.
 *
 * Available variables:
 * - $image: status image
 * - $alt: 'alt' message
 * - $title: 'title' message
 *
 * @see template_preprocess_simplenews_status()
 */
?>
  <?php if (isset($image)): ?>
    <img src="<?php print $image; ?>" width="15" height="15" alt="<?php print $alt; ?>" border="0" title="<?php print $title; ?>" />
  <?php else: ?>
    <?php print $title; ?>
  <?php endif; ?>
