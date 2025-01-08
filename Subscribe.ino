void Ssubscribe(){
client.subscribe("request/currentData");
client.subscribe("request/currentDataBasic");
client.subscribe("request/dataEdit");
client.subscribe("request/hoursView");

client.subscribe("server_response");
client.subscribe("server_return_date");
client.subscribe("server_restore");
client.subscribe("server_restore_current");

}