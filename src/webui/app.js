'use strict';

angular.module('ACTubeManApp', ['ngAnimate', 'ngRoute'])

  .config(function($routeProvider) {
    $routeProvider
      .when('/', { templateUrl: 'articles.html' })
      .when('/wtps', { template: 'WTPs' })
      .when('/about', { templateUrl: 'about.html' })
      .otherwise({ redirectTo: '/'});
  })


  .directive('price', function(){
    return {
      restrict: 'E',
      scope: {
        value: '='
      },
      template: '<span ng-show="value == 0">kostenlos</span>' +
        '<span ng-show="value > 0">{{value | currency}}</span>'
    }
  })
  .factory('Cart', function() {
    var items = [];
    return {
      getItems: function() {
        return items;
      },
      addArticle: function(article) {
        items.push(article);
      },
      sum: function() {
        return items.reduce(function(total, article) {
          return total + article.price;
        }, 0);
      }
    };
  })
  .controller('ArticlesCtrl', function($scope, $http, Cart){
    $scope.cart = Cart;
    $http.get('./actube.php').then(function(articlesResponse) {
      $scope.articles = articlesResponse.data;
    });
  })
  .controller('CartCtrl', function($scope, Cart){
    $scope.cart = Cart;
  });
