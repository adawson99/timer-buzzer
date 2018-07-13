var client_module_plugin = {
init: (socket,firebaseUser)=>{
  console.log('Initializing client module plugin...');
  //buzzerCard, with image of my circuit
  let buzzerCard = new window.Card("https://cdn.glitch.com/fec77fd8-4c26-4647-bddb-93ff738cc8c7%2FMicrogreenBuzzerCircuit.png?1531419897677", "Piezo Buzzer Status");
  
  //provide info on the state of the buzzer
  socket.on('buzzUpdate',data=>{
    console.log(data);
    buzzerCard.setReadOut(`Buzzer Buzzing State: ${data}`);
  });
  
  // timeSensorCard
  let timeSensorCard = new window.Card("https://cdn.glitch.com/fec77fd8-4c26-4647-bddb-93ff738cc8c7%2FRiAB6RaLT.jpg?1531421446994", "Time Ellapsed Since Watering");
     
  //provides info on the ellapsed time in a easy to read format
  socket.on('timeUpdate',data=>{
    console.log(data);
    timeSensorCard.setReadOut("You haven't watered the microgreens for: " + timeSensorCard.msToTime(data));
  }); 
  
  //sends to the resetTime subscription
  timeSensorCard.addButton("Reset Time", e=>{
    socket.emit("resetTime");
  });
  
  //potential check time button
  /**
  timeSensorCard.addButton("Check Time", e=>{
    socket.emit("checkTime");    
  });
  **/
  
}
}
