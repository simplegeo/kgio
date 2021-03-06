require 'test/unit'
require 'io/nonblock'
$-w = true
require 'kgio'

class TestConnectFDLeak < Test::Unit::TestCase

  def test_unix_socket
    nr = 0
    path = "/non/existent/path"
    assert(! File.exist?(path), "#{path} should not exist")
    assert_nothing_raised do
      begin
        sock = Kgio::UNIXSocket.new(path)
      rescue Errno::ENOENT
      end while (nr += 1) < 10000
    end
  end
end
