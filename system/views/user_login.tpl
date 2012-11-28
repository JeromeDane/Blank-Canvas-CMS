<h1>User Login</h1>

{form}
	{form_errors}
	<table class="user_login_layout" style="margin-top:2em;">
		<tr>
			<th>{label for="email"}Email{/label}</th>
			<td>{input name="email" email="You must use your email address to login"}</td>
		</tr>
		<tr>
			<th>Password</th>
		 	<td><input name="password" type="password"/></td>
		</tr>
		{if $numLoginAttempts >= 3}
			<tr>
				<th></th>
			 	<td>{html_captcha}</td>
			</tr>
		{/if}
		<tr>
			<th></th>
		 	<td>
		 		{if $loginError}
		 			<p style="color:red; font-weight:bold;" class="error">
		 				Invalid login combination
		 			</p>
		 		{/if}
		 		
		 		{submit value="Login" name="login"}
		 	</td>
		</tr>
	</table>
{/form}
