module.exports = function() {
  var clayConfig = this;


  function onBtnClick() {
    clayConfig.config[1].items[1].options = [
      { "label": "Dynamic 1", "value": "1" },
      { "label": "Dynamic 2", "value": "2" },
      { "label": "Dynamic 3", "value": "3" }
    ]

    clayConfig.build();
  }

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    clayConfig.getItemById('triggerBtn').on('click', onBtnClick);

    clayConfig.getItemById('selectionGroup').on('change', function() {
      clayConfig.getItemById('textDisplay').set(this.get());
    });


  });

}
