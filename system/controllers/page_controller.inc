<?php

namespace BlankCanvas;


class PageController extends Controller {
	
	public function index() {
		
		
		return $this->view();
	}
	public function numbers($params = null) {
		
		$params['p'] = App::getRequestVar('p') ? App::getRequestVar('p') : 1;
		
		
		// generate url
		$params['url'] = preg_replace('/\/p_\d+/', '', App::getRequestUri());
		$params['url'] = preg_replace('/\/$/', '', $params['url']);
		
		// generate num pages to show in page list
		$params['numPagesInList'] = 10;
		
		// figure out how many pages there are in total
		$params['numPagesTotal'] = ceil($params['total'] / $params['per_page']);

		$params['numToLeft'] = floor($params['numPagesInList'] / 2);
		$params['numToRight'] = ceil($params['numPagesInList'] / 2) - 1;
		
		// figure out the starting page number
		$params['startingPageNum'] = $params['p'] - $params['numToLeft'];
		
		// figure out ending page number
		$params['endingPageNum'] = $params['p'] + $params['numToRight'];
		
		// tack unused pages to the left onto the number displayed to the right
		if(($numPagesUnused = $params['p'] - $params['numToLeft'] - 1) < 1) {
			$params['startingPageNum'] = 1;
			$params['endingPageNum'] = $params['numPagesInList'];
		} else if($numPagesUnused == 1) {
			$params['startingPageNum'] = 1;
			$params['endingPageNum'] = $params['numPagesInList'];
		}
		
		// tack unused pages to the right onto the number displayed to the left
		if(($numPagesUnused = $params['numToRight'] - ($params['numPagesTotal'] - $params['p'])) > 0) {
			$params['startingPageNum'] -= $numPagesUnused;
			$params['endingPageNum'] = $params['numPagesTotal'];
		} else if($numPagesUnused == -1) {
			$params['startingPageNum'] -= $numPagesUnused;
			$params['endingPageNum'] = $params['numPagesTotal'];
		}
		
		if($params['startingPageNum'] < 1 || $params['startingPageNum'] > $params['numPagesTotal']) {
			$params['startingPageNum'] = 1;
		}
		if($params['endingPageNum'] < 1 || $params['endingPageNum'] > $params['numPagesTotal']) {
			$params['endingPageNum'] = $params['numPagesTotal'];
		}
				
		return $this->view($params, false);
	}
	
	
	
}


?>