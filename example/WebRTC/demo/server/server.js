var ws = require("nodejs-websocket");
var port = 8099;

var server = ws
  .createServer(function (conn) {
    console.log("创建一个新连接");
    conn.sendText("连接成功");
    conn.on("text", function (str) {
      console.info("recv msg : " + str);
    });
    conn.on("close", function (code, reason) {
      console.info("close : " + code + ", reason : " + reason);
    });
    conn.on("error", function (err) {
      console.info("error : " + err);
    });
  })
  .listen(port);
