<?php
// $Id: page-user.tpl.php,v 1.1.4.4 2009/08/05 18:55:54 add1sun Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <link href="/sites/scientists.org.nz/themes/sci/css/profile.css" rel="stylesheet" type="text/css" />
  <?php print $scripts; ?>
<script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
<!-- Google analytics -->
<script type="text/javascript">

  var _gaq = _gaq || [];
  _gaq.push(['_setAccount', 'UA-23616948-1']);
  _gaq.push(['_trackPageview']);

  (function() {
    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
  })();

</script>
  
</head>
  <body>

    <!-- header starts-->
    <div id="header-wrap">
      <div id="header" class="container_16">
        <div id="header-main">
          <h1 id="logo-text"><a href="<?php print $front_page; ?>"><?php print $site_name; ?></a></h1>
          <p id="slogan"><?php print $site_slogan; ?></p>
        </div><!-- navigation -->
        <div id="nav">
        
          <?php print theme('links', $primary_links, array('class' => 'links primary-links')); ?>
        </div>
           <div id="nav2">
         <?php if ($secondary_links): ?>
            <ul class="links secondary-links"><li><?php global $user; print l("My Profile","user/$user->uid"); ?></li></ul>
          <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')); ?>  <?php endif; ?>
        </div>
      </div>
    </div>
    <!-- header ends here -->
    
    <!-- content starts -->
   
    <div id="content-wrapper" class="container_16">

<!--      <div id="breadcrumb" class="grid_16"><?php print $breadcrumb; ?></div>-->
  <div id="breadcrumb" class="grid_16"><?php print $bcrumb; ?></div>
      <!-- main -->
      <div id="main" class="<?php print ($left && $right) ? 'grid_8' : (($left || $right) ? 'grid_12' : 'grid_16') ?>">
 <!--       <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
       <?php if (!empty($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>--> 
        <?php if (!empty($messages)): print $messages; endif; ?>
        <?php if (!empty($help)): print $help; endif; ?>
        <div id="content-output">
<!--
<h1 class="title" id="page-title">Join NZAS</h1>
<p> 
Your membership helps support the advocacy of science in New Zealand. As a member of the NZAS you also receive:<br />
<ul>
<li>New Zealand Science Review, the official publication of the association, </li>
<li>an optional profile on scientists.org.nz,</li>
<li>representation before government, and other relevant groups, on issues of importance to scientists, and </li>
<li> opportunities for participation in discussion on issues relating to New Zealand science.</li>
</ul>
</p>
<p>
Annual membership fees are:<br />
<ul>
<li>$65.00 for individuals; </li>
<li>$75.00 for joint members; </li>
<li>$42.00 for students, the unwaged or retired; </li>
<li>$121.00 for corporate membership (receive 2 copies of NZSR). </li>
</ul>
</p>
<p>
Further information on membership of the NZAS may be obtained by contacting the membership secretary by email (membership@scientists.org.nz), or by writing to the New Zealand Association of Scientists, P.O. Box 1874, Wellington. To join now please complete the following information:
</p>
-->
<p>
Please complete the following information:
</p> 
          <?php print $content; ?>
        </div><!-- /#content-output -->
      </div>
      
      <!-- main ends here -->

      <!-- sidebars starts here -->
      <?php if ($left || $right): ?>
      <div id="sidebars" class="<?php print ($left && $right) ? 'grid_8' : 'grid_4' ?>">

        <!-- left sidebar starts here -->
        <?php if ($left): ?>
        <div class="grid_4 alpha sidebar-left">
          <?php print $left; ?>
        </div>
        <?php endif; ?>
        <!-- left sidebar ends here -->

        <!-- right sidebar starts here -->
        <?php if ($right): ?>
        
        <div class="grid_4 omega sidebar-right">
          <?php print $right; ?>
        </div>
        <?php endif; ?>
        <!-- right sidebar ends here -->

      </div>
      <?php endif; ?>
      <!-- sidebars end here -->

    </div>
    <!-- content ends here -->

    <!-- footer starts here -->
    <div id="footer-wrapper" class="container_16">

      <!-- footer top starts here -->
      <div id="footer-content">

        <!-- footer left starts here -->
        <div class="grid_8" id="footer-left">
          <?php print $footer_left; ?>
        </div>
        <!-- footer left ends here -->

        <!-- footer right starts here -->
        <div class="grid_8" id="footer-right">
          <?php print $footer_right; ?>
        </div>
        <!-- footer right ends here -->

      </div>
      <!-- footer top ends here -->

      <!-- footer bottom starts here -->
      <div id="footer-bottom">
        <div id="footer-meta" class="clear-block">
          <?php if ($footer_message): ?>
          <p class="bottom-left"><?php print $footer_message; ?></p>
          <?php endif; ?>
  
          <?php if ($secondary_links): ?>
          <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')); ?>
          <?php endif; ?>
        </div>

        <?php if ($footer): ?>
        <div id="footer-bottom-content">
          <?php print $footer; ?>
        </div>
        <?php endif; ?>
      </div>
      <!-- footer bottom ends here -->

    </div>
    <!-- footer ends here -->
    <?php print $closure; ?>
  </body>
</html>
