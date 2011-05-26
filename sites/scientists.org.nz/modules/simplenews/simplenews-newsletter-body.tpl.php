<?php
// $Id: simplenews-newsletter-body.tpl.php,v 1.3 2009/01/02 12:01:17 sutharsan Exp $

/**
 * @file
 * Default theme implementation to format the simplenews newsletter body.
 *
 * Copy this file in your theme directory to create a custom themed body.
 * Rename it to simplenews-newsletter-body--<tid>.tpl.php to override it for a 
 * newsletter using the newsletter term's id.
 *
 * Available variables:
 * - node: Newsletter node object
 * - $body: Newsletter body (formatted as plain text or HTML)
 * - $title: Node title
 * - $language: Language object
 *
 * @see template_preprocess_simplenews_newsletter_body()
 */
?>
<a href="http://www.scientists.org.nz" target="_blank"><img src="http://www.scientists.org.nz/sites/scientists.org.nz/themes/sci/images/header_small.gif" width="254" height="57" border="0" align="right" title="NZAS Header" ></a><br />

<h2><?php print $title; ?></h2>

<?php print $body; ?>
