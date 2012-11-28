{if $total > $per_page}
	<span class="page_numbers_label">Page:</span> 
	
	{if $startingPageNum > 2}
		<a href="{$url}/">1</a>
		&#133;
	{/if}
	
	{for $i = $startingPageNum; $i <= $endingPageNum; $i++}
		{if $i == $p}
			<span class="page_num_selected">{$i}</span>
		{else}
			<a href="{$url}/p_{$i}">{$i}</a>
		{/if}
	{/for}
	
	{if $endingPageNum < $numPagesTotal - 1}
		&#133;
		<a href="{$url}/p_{$numPagesTotal}">{$numPagesTotal}</a>
	{/if}
	
{/if}