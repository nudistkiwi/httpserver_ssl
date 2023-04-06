var btnLogin = document.getElementById('do-login');
var idLogin = document.getElementById('change');
var username = document.getElementById('username');

btnLogin.addEventListener("click", function(e) {
  e.preventDefault();
  var usernameValue = document.getElementById("username").value;
  var password = document.getElementById("password").value;

  // Update the login message
  idLogin.innerHTML = '<p>We\'re happy to see you again, </p><h1>' + usernameValue + '</h1>';

  var xhr = new XMLHttpRequest();
  xhr.open("POST", "", true);
  xhr.setRequestHeader("Content-Type", "text/event-stream");
  xhr.onreadystatechange = function() {
    if (xhr.readyState === 4 && xhr.status === 200) {
      var data = JSON.parse(xhr.responseText);
      if (data.status === "success") {
        //alert("Login Successful!");
        window.location.href = "index.html";
      } else {
        //alert("Login Failed. Please try again.");
        window.location.href = "index.html";
      }
    }
  };
  
  //var loginData = JSON.stringify({username: usernameValue, password: password});
  var loginData = {};
  loginData = JSON.stringify({username: usernameValue, password: password});
  xhr.send(loginData);

});


idLogin.addEventListener("click", function(e) {
  e.preventDefault();
  var usernameValue = document.getElementById("username").value;
  var password = document.getElementById("password").value;
  var newpassword = document.getElementById("newpassword").value;
  var cnewpassword = document.getElementById("cnewpassword").value;

  // Update the login message
  idLogin.innerHTML = '<p>We\'re happy to see you again, </p><h1>' + usernameValue + '</h1>';

  var xhr = new XMLHttpRequest();
  xhr.open("POST", "", true);
  xhr.setRequestHeader("Content-Type", "application/json");
  xhr.onreadystatechange = function() {
    if (xhr.readyState === 4 && xhr.status === 200) {
      var data = JSON.parse(xhr.responseText);
      if (data.status === "success") {
        //alert("Login Successful!");
        window.location.href = "index.html";
      } else {
        //alert("Login Failed. Please try again.");
        window.location.href = "index.html";
      }
    }
  };
  
  //var loginData = JSON.stringify({username: usernameValue, password: password});
  var loginData = {}; 
  loginData = JSON.stringify({username: usernameValue, password: password, new_password: newpassword, confirm_new_password: cnewpassword});
  xhr.send(loginData);

});


document.getElementById("do-change").addEventListener("click", function(){
  var newPassword = document.getElementById("new-password");
  var button = document.getElementById("do-login");
  var dobutton = document.getElementById("do-change");
  var change = document.getElementById("change");
  if (newPassword.style.display === "none") {
    newPassword.style.display = "block";
  } else {
    newPassword.style.display = "none";
  }
  var cnewPassword = document.getElementById("confirm-new-password");
  if (cnewPassword.style.display === "none") {
    cnewPassword.style.display = "block";
  } else {
    cnewPassword.style.display = "none";
  }
  if (change.style.display === "none") {
    change.style.display = "block";
    button.style.display = "none";
    dobutton.style.display = "none";
  } else {
  }



});
