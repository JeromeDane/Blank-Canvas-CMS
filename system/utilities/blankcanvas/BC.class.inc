<?php

require 'system/utilities/blankcanvas/BCController.class.inc';
require 'system/utilities/smarty/libs/Smarty.class.php';

/**
 *
 * Enter description here ...
 * @author Jerome
 *
 */
class BC {
	
	/**
	 *
	 */
	protected static $_systemTemplateTags = array(
		'version' => '3.0.0a'
	);
	
	// @todo document addStylesheet
	public static function addStylesheet($source) {
		if (is_file($source)) {
			self::$_stylesheets[$source] = true;
		} else {
			throw new Exception('Can\'t find stylesheet at "' . $source . '"');
		}
	}
	
	/**
	 * Call Action
	 * Calls the current controller action
	 * @param string $actionName the name of the action to use
	 * @return void
	 */
	public static function callAction($actionName, array $params = null) {
		if (!method_exists(self::$_controller, $actionName)) {
			if(self::$_controllerName != '404') {
				BC::setController('404');
			} else {
				BC::callAction('index');
			}
		} else {
			self::$_actionName = $actionName;
			echo (self::$_controller->$actionName($params));
		}
	}
	
	/**
	 * Initialize
	 *
	 * Initializes the system. Only called once by /index.php
	 *
	 */
	public static function init() {
		
		// connect to the database
		require 'config/db.config.inc';
		require 'system/utilities/phpdatamapper/Base.php';
		$adapter = new phpDataMapper_Adapter_Mysql($db['server'], $db['database'], $db['user'], $db['password']);
		
		
		require 'system/models/User.model.inc';
		
		$userModel = new SystemUserModel($adapter);
		$userModel->migrate();
		
		
		BC::setRequestUri();
		BC::setController(BC::getRequestToken(0));
	}
	
	public static function getRequestUri() {
		$location = preg_replace('/\/404.php\?404;https?:\/\/' . $_SERVER['HTTP_HOST'] . ':[0-9]*/i', '', $_SERVER['REQUEST_URI']);
		// strip root path
		$escapedRootUri = str_replace('/', '\/', preg_quote(BC::getRootUri()));
		$location = preg_replace('/^' . $escapedRootUri . '/', '', $location);
		return '/' . $location;
	}
	
	/**
	 * Get Root
	 * Returns the site's root URL path not including the domain
	 *
	 * @return string site URL root without the leading http://example.com
	 */
	public static function getRootUri() {
		static $root;
		if (is_null($root)) {
			preg_match('/^(.*?\/)index.php/i', $_SERVER['PHP_SELF'], $matches);
			$root = $matches ? $matches[1] : '/';
		}
		return $root;
	}
	
	/**
	 * Get Request Token
	 *
	 * @param int $index
	 */
	public static function getRequestToken($index) {
		$tokens = self::$_requestTokens;
		return isset($tokens[$index]) ? $tokens[$index] : null;
	}
	
	/**
	 * Set Controller
	 * Sets the current controller
	 * @param string $controllerName the name of the controller to use
	 * @return void
	 */
	public static function setController($controllerName) {
		$controllerName = strtolower($controllerName);
		$systemControllerFile = 'system/controllers/' . $controllerName . '.controller.inc';
		if (is_file($systemControllerFile)) {
			require_once $systemControllerFile;
			// make sure controller class exists in file
			$className = ucfirst($controllerName) . 'Controller';
			if (!class_exists($className) && $controllerName != '404') {
				throw new Exception('Controller file found at /' . $systemControllerFile . ' but it
									does not contain a valid controller class named "' . $className . '"');
			}
			$controller = $controllerName == '404' ? new FileNotFoundController() : new $className();
			if (!is_subclass_of($controller, "BCController")) {
				throw new Exception('"' . $className . '" class found in /' . $systemControllerFile . ' but it does not extend from BCController');
			}
		} else {
		}
		if (!isset($controller)) {
			BC::setController('404');
		} else {
			self::$_controller = $controller;
			self::$_controllerName = $controllerName;
			
			// define default routes and call action accordingly
			$token = BC::getRequestToken(1);
			if (is_null($token)) {
				BC::callAction('index');
			} else if (preg_match('/^\d+$/', $token, $matches)) {
				BC::callAction('details', array('id' => (int) $matches[0]));
			} else {
				BC::callAction($token);
			}
		}
	}
	
	// @todo document setPageTitle
	public static function setPageTitle($title) {
		if (!isset(self::$_systemTemplateTags['page'])) {
			self::$_systemTemplateTags['page'] = array();
		}
		self::$_systemTemplateTags['page']['title'] = $title;
	}
	/**
	 * Set Request
	 * Sets the system's request URI
	 * @param string $request
	 * @return void
	 */
	public static function setRequestUri($requestUri = null) {
		self::$_requestUri = is_null($requestUri) ? BC::getRequestUri() : $requestUri;
		self::$_requestVars = array();
		// generate rough tokens
		$roughTokens = array();
		if (preg_match('/^\/(index\.php)?$/', self::$_requestUri)) { // on the home page
			$roughTokens = array(
				'index'
			);
		} else {
			$roughTokens = explode('/', self::$_requestUri); // handle normal urls
		}
		// parse rough tokens
		$tokens = array();
		foreach ($roughTokens as $roughToken) {
			if (trim($roughToken) != '') {
				$tokens[] = trim(urldecode(preg_replace('/(%2f|\/)/i', '', $roughToken)));
			}
		}
		self::$_requestTokens = $tokens;
		foreach (self::$_requestTokens as $token) {
			if (preg_match('/^([^_]+)_(.+)$/', $token, $matches)) {
				self::$_requestVars[$matches[1]] = $matches[2];
			}
		}
	}
	
	/**
	 *
	 * Parses and outputs the view
	 */
	public static function view($model = null) {
		
		
		BC::addStylesheet('skins/default/skin.css');
		
		$bodyFile = 'system/views/' . self::$_controllerName . '/' . self::$_actionName . '.tpl';
		if(is_file($bodyFile)) {
			
			$body = new Smarty();
			
			$body->assign($model);
			$body->assign('system', self::$_systemTemplateTags);
			
			$bodyContents = $body->fetch($bodyFile);
			
			$skin = new Smarty();
			
			if ($bob = $body->getTemplateVars('foo')) {
				$skin->assign('foo', $bob);
			} else {
				$skin->assign('foo', 'noooooooooo');
			}
			
			self::$_systemTemplateTags['body'] = $bodyContents;
			
			$skin->assign('system', self::$_systemTemplateTags);
			
			$output = $skin->fetch('skins/default/skin.tpl');
			$output = self::_addStylesToSkinHtml($output);
			
			
			return $output;
		} else {
			throw new Exception("BC::view() was called but no view template could be found");
		}
	}
	
	protected static $_actionName;
	protected static $_controller;
	protected static $_controllerName;
	
	/**
	 * Container for the current request uri
	 * @var string
	 */
	protected static $_requestUri;
	
	/**
	 * Container for the current request tokens
	 * @var array
	 */
	protected static $_requestTokens;
	
	/**
	 * Container for the current request variables
	 * @var array $_requestVars
	 */
	protected static $_requestVars;
	
	// @todo docuemnt $_stylesheets
	protected static $_stylesheets = array();
	
/**
	 * Add styles to skin html
	 *
	 * Takes a string representing the HTML code for the template and inserts the
	 * contents of all CSS files into the <head> tag. This is done in order to ensure
	 * that root level urls are correctly parsed.
	 * @param string $skinHtml The HTML string for the skin template
	 * @return string
	 */
	private static function _addStylesToSkinHtml($skinHtml)
	{
		$style = '';
		$links = '';
		foreach (self::$_stylesheets as $src => $trash)
		{
			if (is_file($src))
			{
				$styleContents = file_get_contents($src);
				if (BC::getRootUri() != '/')
				{
					$style .= "\n\t" . '<style type="text/css">' . preg_replace('/url\(("|\')?\//i', "url(\$1" . BC::getRootUri(), $styleContents) . '</style>' . "\n";
				} else
				{
					$links .= "\n\t" . '<link rel="stylesheet" type="text/css" href="/' . $src . '" /> ';
				}
			}
		}
		$outputHtml = preg_replace('/<\/head>/i', $style . '</head>', $skinHtml, 1);
		$outputHtml = preg_replace('/<\/head>/i', $links . '</head>', $outputHtml, 1);
		return $outputHtml;
	}
	
}

?>