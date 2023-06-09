import scrapy


class NewsSpider2(scrapy.Spider):
    name = "news2"

    start_urls = [
        'http://finance.people.com.cn/n1/2022/0531/c1004-32434655.html'
    ]

    def parse(self, response):
        for news2 in response.css('div.main'):
            if news2.css('h1::text').get() is not None:
                yield {
                    'url': response.url,
                    'title': news2.css('h1::text').get(),
                    'news_content': ' '.join(news2.css('div.rm_txt_con p::text').getall()),
                }
        anchors_a = response.css('div.rm_relevant a')
        yield from response.follow_all(anchors_a, callback=self.parse)