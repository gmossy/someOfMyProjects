var remoteme;

function setup(){

	$('table').on('click', function() {
		readDataNow();
	});


	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: undefined,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});
}


function readDataNow(){
	remoteme.sendUserSyncMessageWebSocket(temperatureArduinoDeviceId,[],onResponse);
}

function webSocketConnectionChange(state){

	if (state==WebsocketConnectingStatusEnum.CONNECTED){
		readDataNow();
	}
}
function onResponse(output){
	var data = new RemoteMeData(output);


	var temp = data.popFloat32();

	var pressure = data.popFloat32();
	var humm = data.popFloat32();
	$("#tempOut").html(temp.toFixed(2)+" C");
	$("#pressOut").html((pressure/100).toFixed(2)+" hPa");
	$("#hummOut").html(humm.toFixed(2)+" %");


}


