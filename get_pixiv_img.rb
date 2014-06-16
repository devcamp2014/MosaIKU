# -*- coding: utf-8 -*-

require 'uri'
require 'open-uri'
require 'csv'
require 'set'

class PixivImg
  def initialize()
    @urlset = Set.new
  end

  def search_url(keyword, page)
    URI.escape("http://spapi.pixiv.net/iphone/search.php?&s_mode=s_tag&word=#{keyword}&order=date&PHPSESSID=0&p=#{page}")
  end

  def download_img(url)
    filename = File.basename(url)

    open(filename, 'wb') do |output|
      open(url, 'rb') do |input|
        output.write(input.read)
      end
    end
  end

  public
  def get_images(keyword)
    (1..100).each do |page|
      open(search_url(keyword, page)) do |f|
        CSV.parse(f.read).each do |e|
          url = e[6]
          return if @urlset.include?(url)
          
          puts url
          @urlset.add(url)
          download_img(url)
          sleep 3 # ゆっくりダウンロードしないと多分怒られる
        end
      end
    end
  end
end

pi = PixivImg.new
pi.get_images("初音ミク")
pi.get_images("鏡音リン")
