let serverModulePlugin = {
  addHandlers: (io,socket,fb,membersOnline,mailService)=>{
    console.log('Adding server module plugin...');
    
    //subscribe to the state of the buzzer
    fb.subscribeTo('buzz',data=>{
      let buzz = data.val();
      console.log(buzz);  
      io.emit('buzzUpdate',buzz);
    });  
    
    // subscribe to the state of the button
    fb.subscribeTo('buttonState', data=>{
      let button = data.val();
      console.log(button);
      io.emit('buttonStateUpdate', button);
    }); 

    // subscribe to the start time of the timer, when the startTime changes, the button must have been pressed, so the startTime is set to the current time
    fb.subscribeTo('startTime', data=>{
      let start = data.val();
      console.log(start);
      fb.set('startTime', Date.now());
    }); 
    
  // possible button to check the time, but requires manual checking
    /**
    socket.on('checkTime', ()=>{
      var start;
      fb.subscribeTo('startTime', data=>{
        start = data.val();
        console.log(start);
    });
      let ellapsed = Date.now() - start;
      if(ellapsed >= 10000) {
        fb.set('timeIsUp', true);
      }
        io.emit('timeUpdate', ellapsed);
        //io.emit('start', start);
    });
    **/
    
  // subscribes to the current time, this is constantly comparing the startTime with the currentTime
  // a time update is emitted with the ellapsed time data
    fb.subscribeTo('currentTime', data=>{
      let current = data.val();
      var start;
      fb.subscribeTo('startTime', data=>{
        start = data.val();
        console.log(start);
    });
      let ellapsed = current - start;
      // when the ellapsed time is high enough, the timeIsUp = true
      if(ellapsed >= 150000) {
        fb.set('timeIsUp', true);
      }
        io.emit('timeUpdate', ellapsed);
    });
    
    //resets the time when the button online is pressed, resets the startTime, buzz, and timeIsUp
    socket.on('resetTime', ()=> {
      fb.set('timeIsUp', false);
      fb.set('startTime', Date.now());
      fb.set('buzz', false);
    });
  
    var myInterval = setInterval(function (){ fb.set('currentTime', Date.now());}, 1000); 
    //set the time to the current time every second
  }
}
module.exports = serverModulePlugin;




