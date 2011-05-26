
<?php
$author = user_load(array('uid' => arg(1)));
?>

<div id="profile">
   <div id="profile_pic">
        <?php print $profile[user_picture]; ?>
   </div>
        <div id="profile_name">
         <?php print $author->profile_title;  ?> 
            <?php print $author->profile_fname;  ?> 
			<?php print $author->profile_sname;  ?>
            <div id="clear"></div>
            <div id="profile_qual"><?php print $author->profile_qual;  ?></div>
        </div>
        <div id="profile_affiliation">
           <?php print $author->profile_position;  ?><br />
           <a href="<?php print $author->profile_inwebpage;  ?>" target="_blank"><?php print $author->profile_affiliation;  ?></a><br/>
        
                </div>     
   
   
  
      <?php if (empty($author->private_profile_mailaddress)): ?> 
    <div id="profile_more">
        <div id="profile_about">
        <strong>Mailing address:</strong>
         <?php print $author->profile_mailaddress; ?>
        </div>
           </div>
   <?php endif; ?>
       <?php if (empty($author->private_profile_telephone)): ?>    
    <div id="profile_more">
        <div id="profile_about">
        <strong>Telephone:</strong>
         <?php print $author->profile_telephone; ?>
        </div>
           </div>
   <?php endif; ?>
 </div>
  <?php if (!empty($author->profile_about)): ?> 
  <?php if (empty($author->private_profile_about)): ?>   
    <div id="profile_more">
        <div id="profile_about">
        <h2>About me</h2>
        <?php print $author->profile_about; ?>
        </div>
         </div>
   <?php endif; ?>
     <?php endif; ?>

  <div id="profile_mem">
 <?php print $profile ['Membership']; ?>
  </div>
    




