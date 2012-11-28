<?php

function smarty_modifier_number($value, $param1 = null) {
	return number_format($value);
}

?>