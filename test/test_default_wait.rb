require 'test/unit'
require 'io/nonblock'
$-w = true
require 'kgio'

class TestDefaultWait < Test::Unit::TestCase

  def test_socket_pair
    a, b = Kgio::UNIXSocket.pair
    assert_equal a, a.kgio_wait_writable
    a.syswrite('.')
    assert_equal b, b.kgio_wait_readable
  end

  def test_pipe
    a, b = Kgio::Pipe.new
    assert_equal b, b.kgio_wait_writable
    b.syswrite('.')
    assert_equal a, a.kgio_wait_readable
  end
end
