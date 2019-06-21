var mosca = require('mosca') //importa o mosca

//configura��es do broker
var settings = {
	port: 1883, //porta de opera��o do MQTT
	host: '192.168.10.7', //endere�o do servidor
 	http: { // Servidor de WebSockets
    	port: 81,
    	bundle: true,
    	static: './'
  	}
};
/*
var server = new mosca.Server(settings, function() {
  console.log('Mosca server is up and running');
});
*/
var server = new mosca.Server(settings); //cria um broker MQTT com base nas configura��es

//evento ocore quando um cliente se conecta ao broker
//a fun��o .on() permite ouvir programaticamente mensagens recebidas do lado do cliente
server.on('clientConnected', function(cliente){
	console.log("Cliente Conectado! id = ", cliente.id);
});

//evento ocorre quando uma mensagem � publicada
server.on('published', function(packet, cliente){
	console.log("T�pico: ", packet.topic, " | ", new Date().toISOString());// Exibe uma mensagem com o t�pico da mensagem recebida
});
server.on('published', function(packet, client) {
  console.log('Mensagem : ', packet.payload.toString());
});
server.on('ready', setup);//inicia o broker
function setup(){
	console.log("Server is up and running");
}
