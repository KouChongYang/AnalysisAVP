"use strict";

var LocalVideo = document.querySelector("#LocalVideo");
var RemoteVideo = document.querySelector("#RemoteVideo");
var LocalStream = null;

var RTCEngine;
var RTCEngine = function (wsurl) {
  this.init(wsurl);
  RTCEngine = this;
  return this;
};
RTCEngine.prototype.init = function (wsurl) {
  this.wsurl = wsurl;
  this.signaling = null;
};
RTCEngine.prototype.createWebsocket = function () {
  RTCEngine = this;
  RTCEngine.signaling = new WebSocket(this.wsurl);
  RTCEngine.signaling.onopen = function () {
    RTCEngine.onopen();
  };
  RTCEngine.signaling.onmessage = function (ev) {
    RTCEngine.onmessage(ev);
  };
  RTCEngine.signaling.onerror = function (ev) {
    RTCEngine.onerror(ev);
  };
  RTCEngine.signaling.onclose = function (ev) {
    RTCEngine.onclose(ev);
  };
};
RTCEngine.prototype.onopen = function () {
  console.log("websocket open");
};
RTCEngine.prototype.onmessage = function (ev) {
  console.log("websocket message : " + ev.data);
};
RTCEngine.prototype.onerror = function (ev) {
  console.log("websocket error : " + ev.data);
};
RTCEngine.prototype.onclose = function (ev) {
  console.log(
    "websocket close : " + ev.code + ", reason : " + EventTarget.reason
  );
};

function openLocalStream(stream) {
  LocalVideo.srcObject = stream;
  LocalStream = stream;
}

function initLocalStream() {
  navigator.mediaDevices
    .getUserMedia({
      audio: true,
      video: true,
    })
    .then(openLocalStream)
    .catch(function (e) {
      alert("getUserMedia error : " + e.name);
    });
}

RTCEngine = new RTCEngine("ws://127.0.0.1:8099");
RTCEngine.createWebsocket();

document.getElementById("JoinBtn").onclick = function () {
  console.log("加入按钮被点击");
  initLocalStream();
};
