$Id: README.txt,v 1.11.2.7 2010/04/19 07:27:36 sutharsan Exp $

DESCRIPTION
-----------

Simplenews publishes and sends newsletters to lists of subscribers. Both
anonymous and authenticated users can opt-in to different mailing lists. 
HTML email can be sent by adding Mime mail module.


REQUIREMENTS
------------

 * Taxonomy module
 * For large mailing lists, cron is required.
 * HTML-format newsletters and/or newsletters with file attachments require the
   Mime Mail module or HTML Mail module.

INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews" in the sites/all/modules directory and
    place the entire contents of this simplenews folder in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page:
      Administer > Site building > Modules

 3. ACCESS PERMISSION

    Grant the access at the Access control page:
      Administer > User management > Access control. 

 4. CONFIGURE SIMPLENEWS

    Configure Simplenews on the Simplenews admin pages:
      Administer > Site configuration > Simplenews.

    Select the content type Simplenews uses for newsletters.
    Select the taxonomy term Simplenews uses to manage newsletter series.

 5. ENABLE SIMPLENEWS BLOCK

    With the Simplenews block users can subscribe to a newsletter. For each
    newsletter one block is available.
    Enable the Simplenews block on the Administer blocks page:
      Administer > Site building > Blocks.

 6. CONFIGURE SIMPLENEWS BLOCK

    Configure the Simplenews block on the Block configuration page. You reach
    this page from Block admin page (Administer > Site building > Blocks). 
    Click the 'Configure' link of the appropriate simplenews block.
 
    Permission "subscribe to newsletters" is required to view the subscription
    form in the simplenews block or to view the link to the subscription form.

 7. SIMPLENEWS BLOCK THEMING

    More control over the content of simplenews blocks can be achieved using 
    the block theming. Theme your simplenews block by copying 
    simplenews-block.tpl.php into your theme directory and edit the content.
    The file is self documented listing all available variables.

    The newsletter block can be themed generally and per newsletter:
      simplenews-block.tpl.php (for all newsletters)
      simplenews-block.tpl--[tid].php (for newsletter series tid)

 8. MULTILINGUAL SUPPORT
 
    Simplenews supports multilingual newsletters for node translation,
    multilingual taxonomy and url path prefixes.

    When translated newsletter issues are available subscribers receive the
    newsletter in their preferred language (according to account setting).
    Translation module is required for newsletter translation.

    Multilingual taxonomy of 'Localized terms' and 'per language terms' is
    supported. 'per language vocabulary' is not supported.
    I18n-taxonomy module is required.
    Use 'Localized terms' for a multilingual newsletter. Taxonomy terms are
    translated and translated newsletters are each taged with the same
    (translated) term. Subscribers receive the newsletter in the preferred
    language set in their account settings or in the site default language.
    Use 'per language terms' for mailing lists each with a different language.
    Newsletters of different language each have their own tag and own list of
    subscribers.

    To use multilingual confirmation you must use the String translation module.
    Enter confirmation email title and body in ENGLISH at:
      Administer > Site configuration > Simplenews > Subscription.
    If required refresh the Simplenews text group at:
      Administer > Site building > Translate interface > Refresh.
    Enter translations of title and body text at:
      Administer > Site building > Translate interface > Search.

    Path prefixes are added to footer message according to the subscribers
    preferred language.

    The preferred language of anonymous users is set based on the interface
    language of the page they visit for subscription. Anonymous users can NOT
    change their preferred language. Users with an account on the site will be
    subscribed with the preferred language as set in their account settings.

9.  NEWSLETTER THEMING

    You can customize the theming of newsletters. Copy any of the *.tpl.php 
    files from the simplenews module directory to your theme directory. Both
    general and by-newsletter theming can be performed.
    Theme newsletter body:
      simplenews-newsletter-body.tpl.php (for all newsletters)
      simplenews-newsletter-body--[tid].tpl.php (for newsletter series tid;
      where [tid] is replaced by the term id of the newsletter taxonomy term.
    Theme newsletter footer:
      simplenews-newsletter-footer.tpl.php (for all newsletters)
      simplenews-newsletter-footer--[tid].tpl.php (for newsletter series tid)
    The files are self documented listing all available variables.

    Using the Display fields settings each field of a simplenews newsletter can
    be displayed or hidden in plain text and/or HTML newsletters. You find 
    these settings at: 
      admin/content/node-type/my-node-type/display


10. SEND MAILING LISTS

    Cron is required to send large mailing lists. Cron jobs can be triggered
    by Poormanscron or any other cron mechanism such as crontab.
    If you have a medium or large size mailing list (i.e. more than 500
    subscribers) always use cron to send the newsletters.
  
    To use cron:
     * Check the 'Use cron to send newsletters' checkbox.
     * Set the 'Cron throttle' to the number of newsletters send per cron run.
       Too high values may lead to mail server overload or you may hit hosting
       restrictions. Contact your host.
    
    Don't use cron:
     * Uncheck the 'Use cron to send newsletters' checkbox.
       All newsletters will be sent immediately when saving the node. If not 
       all emails can be sent within the available php execution time, the 
       remainder will be sent by cron. Therefore ALWAYS enable cron.

    These settings are found on the Newsletter Settings page under
    'Send mail' options at:
      Administer > Site configuration > Simplenews > Send mail.

 11. (UN)SUBSCRIBE CONFIRMATION
 
    By default the unsubscribe link will direct the user to a confirmation page.
    Upon confirmation the user is directed to the home page, where a message
    will be displayed. On the Simplenews subscription admin page you can
    specify an alternative destination page.
      Administer > Site configuration > Simplenews > Subscription
    To skip the confirmation page you can add parameters to the subscription URL.
      Example: [simplenews-subscribe-url]/ok
    When an alternative destination page has been defined the extra parameters
    will be added to the destination URL.
      Example: [simplenews-subscribe-url]/ok
      Destination: node/123
      Destination URL: node/123/ok

 12. SINGLE OR DOUBLE OPT-IN AND OPT-OUT

    Every newsletter can be set to be double opt-in/out (default), single
    opt-in/out, or hidden.

    Double: A confirmation email is sent to confirm the (un)subscribe action.
            No confirmation is sent when a user is (un)subscribed by the
            administrator or when the user subscribes when creating an account.
    Single: No confirmation email is sent. (un)subscribe is immediately.
    Hidden: The newsletter is not listed in newsletter lists. Use this for
    mandatory newsletters. Only administrators or modules can add a user to this
    mailing list.

    Note that single opt-in/out or hidden (forced) subscription is in some
    countries forbidden by law.

    SECURITY NOTICE: a newsletter set to be single opt-in or opt-out is
    vulnerable to Cross Site Request Forgeries. Email addresses may be
    (un)subscribed without a notice. Do not use this setting in uncontrolled
    environments (like the internet!).
 
 13. TIPS

    A subscription page is available at: /newsletter/subscriptions
    
    If your unsubscribe URL looks like:
      http://newsletter/confirm/remove/8acd182182615t632
    instead of:
      http://www.mysite.org/newsletter/confirm/remove/8acd182182615t632
    You should change the base URL in the settings.php file from
      #  $base_url = 'http://www.example.com';  // NO trailing slash!
    to
      $base_url = 'http://www.mysite.org';  // NO trailing slash!

DOCUMENTATION
-------------
More help can be found on the help pages: example.com/admin/help/simplenews
and in the drupal.org handbook: http://drupal.org/node/197057
