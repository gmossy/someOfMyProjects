var remoteme;

function setup(){

	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: undefined,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});
}




function webSocketConnectionChange(state){

	if (state==WebsocketConnectingStatusEnum.CONNECTED){
		alert("connected to RemotemeApp :)")
	}
}