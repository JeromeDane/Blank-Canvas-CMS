<style type="text/css">
	@IMPORT url("{root}system/css/user_login.css");
</style>
<h1>New Account</h1>
<p>Create a new account for your Google identification.</p>
{form}
	{form_errors}
	<table class="user_login_layout" style="margin-top:2em;">
		<tr>
			<th>{label for="registration_name"}Name{/label}</th>
		 	<td>{input name="registration_name"}
		 		- This is how others will identify you on the site
		 	</td>
		</tr>
		<tr>
			<th></th>
		 	<td>
		 		{submit value="Create Account" name="create"}
		 	</td>
		</tr>
	</table>
{/form}
