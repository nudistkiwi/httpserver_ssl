var btnLogin = document.getElementById('do-login');
var idLogin = document.getElementById('login');
var username = document.getElementById('username');

btnLogin.addEventListener("click", function(e) {
  e.preventDefault();
  var usernameValue = document.getElementById("username").value;
  var password = document.querySelector(".password").value;

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
  var loginData = JSON.stringify({username: usernameValue, password: password});
  xhr.send(loginData);
});