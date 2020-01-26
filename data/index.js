//Selectors
const randomId = document.querySelector('#random-id');
const newRandomIdButton = document.querySelector('#new-random-id-button');

const resetWifi = document.querySelector('#reset_wifi');
const startTest = document.querySelector('#start-test-button');
const resumeTest = document.querySelector('#resume-test-button');
//Selectors
document.addEventListener('WS_id', idHandler, false);

function idHandler() {
  randomId.value = id_value;
}

document.addEventListener("DOMContentLoaded", function(event) {

  startTest.addEventListener('click', function(e) {
    e.preventDefault;
    if(id_value === prev_id_value){
      alert("ERROR\nPlease generate new ID" );
    }
    else {
      startTest.href="video.html";
      return false;
    }
  });

  resumeTest.addEventListener('click', function(e) {
    e.preventDefault;
    if(id_value === prev_id_value){
      resumeTest.href="resume.html";
      return false;
    }
    else {
      alert("ERROR\nNo test in progress,\nPlease start new test");
    }
  });
  
  //Generate new random ID
  newRandomIdButton.addEventListener('click', sendStartData);

  //Reset WiFi configuration
  resetWifi.addEventListener('click', sendResetData);

  
  //Functions
});
