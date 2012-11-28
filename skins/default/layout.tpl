<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>{if $system.page_title}- {$system.page_title}{/if}</title>
<meta name="description" content="" />
<meta name="keywords" content="" />
<script language="javascript" type="text/javascript" src="{root}plugins/ur/javascript/jquery/jquery-1.6.min.js"></script>
<script language="javascript" type="text/javascript" src="{root}plugins/ur/javascript/jqueryui/js/jquery-ui-1.8.12.custom.min.js"></script>
<link rel="stylesheet" type="text/css"  href="{root}plugins/ur/javascript/jqplot/jquery.jqplot.css" />
<link rel="stylesheet" type="text/css"  href="{root}plugins/ur/javascript/jqueryui/css/smoothness/jquery-ui-1.8.12.custom.css" />

<body style="margin:0; padding:0;">

{guest}
	{form style="text-align:right;" action="{root}user/login"}
		<p><strong>Welcome Guest</strong> [<a href="{root}user/register">register</a>]</p>
		<p style="margin-bottom:.5em;">Email {input name="email" style="width:100px;"}</p>
		<p style="margin-bottom:.5em;">Password <input type="password" name="password" style="width:100px;"/></p>
		<p style="color:#900; font-size:.9em;">
			{submit value="Login" name="login"}
		</p>
	{/form}
	
	<div>
	    Connect with 
		<span title="Connect using your Google account"> 
			{view context="googlelogin" controller="button" action="login"}
		</span>
	</div>
{/guest}

{registered}
	<p><strong>Welcome {user_name}!</strong> [<a href="{root}user/logout">logout</a>]</p>
{/registered}


{$system.body nofilter}

<!-- ====================================================================== Loading message ====================== -->
<div id="loading_message" style="display:none; position:absolute; top:0; left:0; z-index:999999">
	<div id="loading_message_box" class="popup_form" style="width:230px; padding:20px;">
		<h1 style="margin:0; font-size:16px;">
			<img src="/skins/default/images/loading.gif" alt="Loading... please wait." align="absmiddle" style="margin-right:10px;" />
			Loading... Please Wait
		</h1>
	</div>
	<div id="loading_message_mask" class="screen_mask"></div>
</div>


</body>
</html>
