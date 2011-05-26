
(function(){
 var a= {
  exec:function(editor){
   var format = {
    element : "h1"
   };
   var style = new CKEDITOR.style(format);
   style.apply(editor.document);
  }
 },

 b="button-pre";
 CKEDITOR.plugins.add(b,{
  init:function(editor){
   editor.addCommand(b,a);
   editor.ui.addButton("heading-h1",{
    label:"Heading",
    icon: this.path + "button-pre.png",
    command:b
   });
  }
 });
})();