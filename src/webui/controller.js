'use strict';
// definieren eines Moduls
var helloWorldModule = angular.module("helloWorldModule", []);
 
// hinzufügen eines Controllers zum Modul
helloWorldModule.controller("HelloWorldController", function ($scope) {
   $scope.name = "World";
});

