//////////////////////////////////////////////
// Global variable define
//////////////////////////////////////////////
var basicURL = "";

//////////////////////////////////////////////
// Global function define
//////////////////////////////////////////////
var doStart = function() {
    var remote = $("remote").val();
    $.ajax({
        type: "GET",
        url: basicURL + "cgi/start",
        cache: false,
        data: "remote=" + remote,
    });
};

var doStop = function() {
    $.ajax({
        type: "GET",
        url: basicURL + "cgi/stop",
        cache: false,
    });
}

//////////////////////////////////////////////
// Top level code define
//////////////////////////////////////////////
$(document).ready(function(){

});

$("#page_main").live("pageinit", function() {
    basicURL = $(location).attr('href');

});

