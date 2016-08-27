function populateSidebar(){
  var xhttp = new XMLHttpRequest(); 
  xhttp.onreadystatechange = function(){
    if(xhttp.readyState == 4 && xhttp.status == 200){
      var contacts = JSON.parse(xhttp.responseText); 
      contacts.forEach(function(entry){
        var div = document.getElementById('sidebar'); 
        div.insertAdjacentHTML('beforeend', '<button class="contact_button" id="' + entry + '" onclick="populateContent(this.id)"> <p>' + entry + '</p> </button>');
      });
        setInterval(function(){
          var xhttp2 = new XMLHttpRequest();
          xhttp2.onreadystatechange = function(){
            if(xhttp2.readyState == 4 && xhttp2.status == 200){
              var contacts2 = JSON.parse(xhttp2.responseText); 
              contacts2.forEach(function(entry){
                var div = document.getElementById('sidebar'); 
                div.insertAdjacentHTML('beforeend', '<button class="contact_button" id="' + entry + '" onclick="populateContent(this.id)"> <p>' + entry + '</p> </button>'); 
              });
            }
          } 
          xhttp2.open("GET", "getNewContacts.php", true); 
          xhttp2.send(); 
                
          var xhttp3 = new XMLHttpRequest(); 
          xhttp3.onreadystatechange = function(){
            if(xhttp3.readyState == 4 && xhttp3.status == 200){
              var contacts3 = JSON.parse(xhttp3.responseText); 
              contacts3.forEach(function(entry){
                document.getElementById(entry).style.backgroundColor = 'red'; 
              });
            }
          }
          xhttp3.open("GET", "getUpdatedContacts.php", true); 
          xhttp3.send(); 
        
        }, 10000);
    }
  };
  xhttp.open("GET", "getContacts.php", true); 
  xhttp.send(); 
}

function populateContent(id){
	var div = document.getElementById('content_win'); 
        div.innerHTML = '<iframe class="content_frame" frameBorder="0" src="'+ id + '.html"></iframe>';
}


function populateSMS(id){
  var xhttp = new XMLHttpRequest(); 
  xhttp.onreadystatechange = function(){
    if(xhttp.readyState == 4 && xhttp.status == 200){
      var messages = JSON.parse(xhttp.responseText); 
      messages.forEach(function(entry){
        var div = document.getElementById('content_win');         
        div.insertAdjacentHTML('beforeend', '<div class="message"> <p>' + entry + '</p> </div>');
        div.scrollTop = div.scrollHeight - div.clientHeight;
      });
      setInterval(function(){
        
        var xhttpu = new XMLHttpRequest(); 
        xhttpu.onreadystatechange = function(){
          if(xhttpu.readyState == 4 && xhttpu.status == 200){
            var messages2 = JSON.parse(xhttpu.responseText); 
            messages2.forEach(function(entry1){
              var div2 = document.getElementById('content_win'); 
              div2.insertAdjacentHTML('beforeend', '<div class="message"> <p>' + entry1 + '</p> </div>'); 
              div2.scrollTop = div2.scrollHeight - div2.clientHeight;
            });
          }
        };
        xhttpu.open("GET", "updateMessages.php?q=" + id, true); 
        xhttpu.send(); 
      }, 10000); 
    }
  };
  xhttp.open("GET", "getMessages.php?q=" + id, true); 
  xhttp.send(); 
}

//function updateSMS(id){
//  var xhttp = new XMLHttpRequest(); 
//  xhttp.onreadystatechange = function(){
//    if(xhttp.readyState == 4 && xhttp.status == 200){
//      var messages = JSON.parse(xhttp.responseText); 
//      messages.forEach(function(entry){
//        var div = document.getElementByID('content_win');
//        div.insertAdjacentHTML('beforeend', '<div class="messages"> <p>' + entry + '</p> </div>'); 
//        div.scrollTop = div.scrollHeight - div.clientHeight;
//      });
//    }
//  };
//  xhttp.open("GET", "updateMessages.php?q=" + id, true); 
//  xhttp.send(); 
//}

function send_msg(id){
  var xhttp = new XMLHttpRequest(); 
  xhttp.onreadystatechange = function() {
    if(xhttp.readystate == 4 && xhttp.status == 200){
      document.getElementById('input').value = '';
    }
  };
  xhttp.open("GET", "sendMessage.php?name=" + id + "&msg=" + document.getElementById('input').value); 
  xhttp.send(); 	
}

