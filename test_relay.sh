#!/bin/bash
# Test script para verificar se o relay está rodando e funcionando

echo "=========================================="
echo "SwissLedClock Email Relay - Test"
echo "=========================================="
echo

RELAY_URL="http://192.168.0.50:9090/send"

# Test 1: Check if relay is online
echo "1️⃣  Checking if relay is online..."
response=$(curl -s -o /dev/null -w "%{http_code}" "$RELAY_URL?test=1" 2>/dev/null)
if [ "$response" = "200" ] || [ "$response" = "500" ]; then
    echo "✅ Relay is ONLINE (HTTP $response)"
else
    echo "❌ Relay is OFFLINE or unreachable"
    echo "   Make sure the relay is running on the Pi:"
    echo "   python3 ~/SwissLedClock/workbench_email/pi_mail_relay.py"
    exit 1
fi

echo

# Test 2: Send a test email
echo "2️⃣  Sending test email..."
TEST_TIME=$(date '+%d/%m/%Y %H:%M:%S')
curl -s "$RELAY_URL?device=SwissLedClock&subject=Test&message=This+is+a+test+email&time=$TEST_TIME" -o /dev/null

if [ $? -eq 0 ]; then
    echo "✅ Test email sent!"
    echo "   Check your inbox in a few seconds..."
else
    echo "❌ Failed to send test email"
    exit 1
fi

echo

# Test 3: Send a special message (like the bedtime alarm)
echo "3️⃣  Sending special bedtime message..."
curl -s "$RELAY_URL?device=SwissLedClock&subject=SLEEP+NOW!&message=RAFA!+Time+to+sleep+NOW+and+STOP+DRINKING!&time=$TEST_TIME" -o /dev/null

if [ $? -eq 0 ]; then
    echo "✅ Bedtime message sent!"
else
    echo "❌ Failed to send bedtime message"
    exit 1
fi

echo

echo "=========================================="
echo "✅ All tests complete!"
echo "=========================================="
echo
echo "📧 Check your email (rafael@rgross.ch) for the test messages"
echo
echo "If you don't receive emails:"
echo "1. Check if relay is running: ps aux | grep pi_mail_relay"
echo "2. Check relay logs in real-time:"
echo "   python3 ~/SwissLedClock/workbench_email/pi_mail_relay.py"
