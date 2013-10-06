function log(msg){
	 $("#msg").append("<br/><b>"+msg+"</b><br/>");
}
function append_result(str){
	$("#result").append(str);
}

$(document).ready(function(){
	if (window["WebSocket"]) {
      conn = new WebSocket("ws://{{$}}/ws");
      conn.onclose = function(evt) {	
         log("Connection closed.");
      };
      conn.onmessage = function(evt) {
      	// alert("Got response: " + evt.data);
       	/*var response = JSON.parse(evt.data);
       	if(response["Opcode"] == 1){
       		// alert("Problems is:" + response.Problems);
       		$('#problems')[0].innerHTML = response.Difficulty.Problems;
       		$('#zeroes')[0].innerHTML = response.Difficulty.Zeroes;

	      	var solution = find_xs(response.Problems, response.Difficulty.Zeroes);
	    	var request = { "Problems": solution, "Query": response.Query, "Opcode": 1};
		   	conn.send(JSON.stringify(request));
	    } else {
	        var endTime = Number(new Date().getTime()); // returns the number of MS since the epoch	
	        $("#search").removeAttr("disabled");
			$("#result").html("<br/><b>Result from server</b><br/>");
			append_result(response["Query"]+"<br/>");
			append_result("<br/><b>Time for solving the puzzle: </b><bPr/>");
			append_result((endTime - startTime)+" ms <br/>");
			append_result("<br/><b>The solution was</b><br/>");
			append_result('"'+response["Result"]+'" <br/>');
	    }*/
     }


    } else {
        appendLog($("<div><b>Your browser does not support WebSockets.</b></div>"));
    }

    var startTime;
    /* 
     * This search function is protected by proof-of-work.
     */
   /* $("input").click(function(e){	
	    var request = {"Opcode": e.data, "Query": $('#search_field').val()};
	    $("#search").attr("disabled", "disabled");
	    startTime = Number(new Date().getTime());
	    conn.send(JSON.stringify(request));  // log("Sending " + JSON.stringify(request));
    });
	$("a#b1").click(function(){
		//alert($(this).id);
  var request = {"Opcode": 1, "Query": "sasd"};
  conn.send(JSON.stringify(request));  // log("Sending " + JSON.stringify(request));
 });
	$("a#b2").click(function(){
  var request = {"Opcode": 2, "Query": "tomat"};
  conn.send(JSON.stringify(request));  // log("Sending " + JSON.stringify(request));
 });
	$("a#b3").click(function(){
  var request = {"Opcode": 3, "Query": "köttbullar"};
  conn.send(JSON.stringify(request));  // log("Sending " + JSON.stringify(request));
 });*/

	$("a.btn").click(function(){
		//alert($(this).id);
    	var q = String($(this).data('code'));
    	//alert(q);
    	//log(q);
  		var request = {"Opcode": 1, "Query": q};
  		conn.send(JSON.stringify(request));  // log("Sending " + JSON.stringify(request));
 	});
	buildJoystick();




});	

function buildJoystick() {
		var joystick = document.createElement("joystick"), joystickbackground = document.createElement("joystickbackground"), joystickhitzone = document.createElement("joystickhitzone");

		/*document.addEventListener("resize", self.handleResize);
		self.handleResize();*/

		joystick.id = "actual_joystick";
		
		joystickhitzone.appendChild(joystick);
		joystickbackground.appendChild(joystickhitzone);
		//elem.appendChild(joystickbackground);
		$('#content')[0].appendChild(joystickbackground);
	 	


		/*pretendArrowKeys: true,*/
	 	var a_joystick = new SQUARIFIC.framework.TouchControl($("#actual_joystick")[0], {
					mindistance: 25,
					maxdistance: 75,
					middleLeft: 25,
					middleTop: 25
				});
	 	/*a_joystick.on("pretendKeydown", function (event) {
	 		var q = "";
	 		q += "Keycode: ";
	 		q+= String(event.keyCode);
	 		var request = {"Opcode": 2, "Query": q};
  			conn.send(JSON.stringify(request));

	 	});*/
		a_joystick.on("joystickMove", function (event) {
	 		var q = "";
	 		var d = Math.min(event.distance, 80);
	 		var ang = (event.angle*2*3.141592365)/360;
	 		var x = Math.cos(ang) * d;
	 		var y = Math.sin(ang) * d;
	 		q += x/80;
	 		q += " ";
	 		q += y/80;
	 		//log(x);
	 		//q+= String(JSON.stringify(event));
	 		//q += event.deltaX;
	 		//q += " ";
	 		//q += event.deltaY;
	 		var request = {"Opcode": 2, "Query": q};
  			conn.send(JSON.stringify(request));

	 	});
}

