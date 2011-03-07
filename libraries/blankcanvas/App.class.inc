<?php
/** {@link http://blankcanvas.me/cms/ Blank Canvas CMS}
 *
 * This file is part of the {@link http://blankcanvas.me/cms/ Blank Canvas CMS}.
 *
 * {@link http://blankcanvas.me/cms/ Blank Canvas CMS} is free software:
 * you can redistribute it and/or modify it under the terms of the
 * {@link http://www.gnu.org/licenses/gpl.html GNU General Public License}
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Jerome Dane <http://blankcanvas.me/contact/>
 * @copyright Copyright (c) 2011, Jerome Dane
 * @license http://www.gnu.org/licenses/gpl.html GNU General Public License
 * @package BlankCanvas
 */

namespace BlankCanvas;

/**
 *
 * Main application controller and system interface
 * @author Jerome Dane
 *`
 */
class App {
	
	/**
	 *
	 */
	protected static $_version = '3.0.0a1pre';
	
	// @todo document addStylesheet
	public static function addStylesheet($source) {
		if (is_file($source)) {
			self::$_stylesheetPaths[$source] = true;
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
			if (self::$_controllerName != '404') {
				App::setController('404');
			} else {
				App::callAction('index');
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
		
		// auto load blank canvas classes
		spl_autoload_register(function($className) {
			if(preg_match('/^BlankCanvas/', $className) || __NAMESPACE__ == 'BlankCanvas') {
				$className = preg_replace('/^BlankCanvas\\\\/', '', $className);
				$filePath = 'libraries/blankcanvas/'.$className.'.class.inc';
				if(is_file($filePath)) {
					require_once $filePath;
					return true;
				}				
			}
		});
		
		// connect to the database
		\ActiveRecord\Config::initialize(function ($cfg) {
			$cfg->set_model_directory('application/models');
			require 'config/db.config.inc';
			$cfg->set_connections(array(
				'development' => 'mysql://' . $db['user'] . ':' . $db['password'] . '@localhost/' . $db['database']));
		});
		
		App::handleRequest();
	}
	
	public static function getRequestUri() {
		$location = preg_replace('/\/404.php\?404;https?:\/\/' . $_SERVER['HTTP_HOST'] . ':[0-9]*/i', '', $_SERVER['REQUEST_URI']);
		// strip root path
		$escapedRootUri = str_replace('/', '\/', preg_quote(App::getRootUri()));
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
	
	public static function getStylesheetPaths() {
		return self::$_stylesheetPaths;
	}
	
	public static function getVersion() {
		return self::$_version;
	}
	
	public static function getPageTitle() {
		return self::$_pageTitle;
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
	
	public static function getRequestTokents() {
		return self::$_requestTokens;
	}
	
	/**
	 * Redirect the user to a new URI
	 *
	 */
	public static function redirect($uri) {
		header("Status: 301 Moved Permanently");
		header('HTTP/1.0 301 Moved Permanently');
		header("Location: " . $uri);
		App::close();
	}
	
	/**
	 * Set Controller
	 * Sets the current controller
	 * @param string $controllerName the name of the controller to use
	 * @return void
	 */
	public static function setController($controllerName) {
		$controllerName = strtolower($controllerName);
		$systemControllerFile = 'application/controllers/' . $controllerName . '.controller.inc';
		if (is_file($systemControllerFile)) {
			require_once $systemControllerFile;
			// make sure controller class exists in file
			$className = '\\BlankCanvas\\'.ucfirst($controllerName) . 'Controller';
			if (!class_exists($className) && $controllerName != '404') {
				throw new Exception('Controller file found at /' . $systemControllerFile . ' but it
									does not contain a valid controller class named "' . $className . '"');
			}
			$controller = $controllerName == '404' ? new FileNotFoundController() : new $className();
			
			if (!is_subclass_of($controller, '\\BlankCanvas\\Controller')) {
				throw new Exception($className . '" class found in /' . $systemControllerFile . ' but it does not extend from BCController');
			}
		} else {
			
			
		}
		if (!isset($controller)) {
			App::setController('404');
		} else {
			self::$_controller = $controller;
			self::$_controllerName = $controllerName;
			
			// define default routes and call action accordingly
			$token = App::getRequestToken(1);
			if (is_null($token)) {
				App::callAction('index');
			} else if (preg_match('/^\d+$/', $token, $matches)) {
				App::callAction('details', array('id' => (int) $matches[0]));
			} else {
				App::callAction($token);
			}
		}
	}
	
	// @todo document setPageTitle
	public static function setPageTitle($title) {
		self::$_pageTitle = $title;
	}
	/**
	 * Set Request
	 * Sets the system's request URI
	 * @param string $request
	 * @return void
	 */
	public static function handleRequest($requestUri = null) {
		self::$_requestUri = is_null($requestUri) ? App::getRequestUri() : $requestUri;
		self::_generateRequestTokens();
		
		// determine controller name

		$controllerName = App::getRequestToken(0);
		$filePath = 'application/controllers/' . $controllerName . '.controller.inc';

		if (is_file($filePath)) {
			
			// make sure controller class exists
			require_once $filePath;
			$className = '\\BlankCanvas\\'.ucfirst($controllerName) . 'Controller';
			if (!class_exists($className) && $controllerName != '404') {
				throw new Exception('Controller file found at /' . $systemControllerFile . ' but it
									does not contain a valid controller class named "' . $className . '"');
			}
			
		} else {
			
			// @todo use plugin controller to check for a plugin else use 'file not found' as below
			
			require_once 'application/controllers/filenotfound.controller.inc';
			$controllerName = 'FileNotFound';
		}
		
		// include model
		$modelFilePath = 'application/models/'.ucfirst($controllerName).'.model.inc';
		if(is_file($modelFilePath))
			require_once $modelFilePath;
		
		$controllerClassName = '\\BlankCanvas\\'.ucfirst($controllerName.'Controller');
		
		// make sure the controller extends from \BlankCanvas\Controller
		$controller = new $controllerClassName();
		if (!is_subclass_of($controller, '\\BlankCanvas\\Controller')) {
			throw new Exception($className . '" class found in /' . $systemControllerFile . ' but it does not extend from BCController');
		}
		
		$params = App::getRequestTokents();
		
		// determine action etc.
		
		if(!App::getRequestToken(1)) {		// index
			$actionName = 'index';
		} else if(preg_match('/^\d+$/', App::getRequestToken(1))) {		//
			$actionName = 'details';
			$params['id'] = (int) App::getRequestToken(1);
		} else {
			$actionName = App::getRequestToken(1);
		}
		
		echo($controller->$actionName($params));
		
		
		// @todo consider removing controller inherits from \BlankCanvas\Controller check to improve performance
		
		
		//App::setController(App::getRequestToken(0));
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
	
	// @todo docuemnt $_stylesheetPaths
	protected static $_stylesheetPaths = array();
	
	// @todo document $_pageTitle
	protected static $_pageTitle;
	
	/**
	 * Add styles to skin html
	 *
	 * Takes a string representing the HTML code for the template and inserts the
	 * contents of all CSS files into the <head> tag. This is done in order to ensure
	 * that root level urls are correctly parsed.
	 * @param string $skinHtml The HTML string for the skin template
	 * @return string
	 */
	private static function _addStylesToSkinHtml($skinHtml) {
		$style = '';
		$links = '';
		foreach (self::$_stylesheetPaths as $src => $trash) {
			if (is_file($src)) {
				$styleContents = file_get_contents($src);
				if (App::getRootUri() != '/') {
					$style .= "\n\t" . '<style type="text/css">' . preg_replace('/url\(("|\')?\//i', "url(\$1" . App::getRootUri(), $styleContents) . '</style>' . "\n";
				} else {
					$links .= "\n\t" . '<link rel="stylesheet" type="text/css" href="/' . $src . '" /> ';
				}
			}
		}
		$outputHtml = preg_replace('/<\/head>/i', $style . '</head>', $skinHtml, 1);
		$outputHtml = preg_replace('/<\/head>/i', $links . '</head>', $outputHtml, 1);
		return $outputHtml;
	}
	
	
	protected function _generateRequestTokens() {
		self::$_requestVars = array();
		// generate rough tokens
		$roughTokens = array();
		if (preg_match('/^\/(index\.php)?$/', self::$_requestUri)) { // on the home page
			$roughTokens = array(
				'index'
			);
		} else {
			$roughTokens = preg_split('/\/|\?/', self::$_requestUri); // handle normal urls
		}
		// parse rough tokens
		$tokens = array();
		foreach ($roughTokens as $roughToken) {
			if (trim($roughToken) != '' && preg_match('/^[\d\w]+$/i', $roughToken)) {
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
}

?>