# Download base image
ARG BASE_IMAGE
FROM ${BASE_IMAGE}

RUN sudo apt-get update && \
    sudo apt-get install -y wget gnupg2 apt-transport-https lsb-release && \
    wget -O - https://xpra.org/gpg.asc | sudo apt-key add - && \
    sudo bash -c "echo \"deb https://xpra.org/ $(lsb_release -cs) main\" > /etc/apt/sources.list.d/xpra.list"

# xpra-html5 is a separate package in xpra 3.x (focal) — without it
# the HTML5 client web root /usr/share/xpra/www is missing and any
# browser hitting http://<host>:${NRP_XPRA_PORT}/ gets HTTP 400
# "this port does not support HTTP requests". xpra 6.x (jammy)
# bundles the HTML5 client into the main xpra package so the same
# install line is a no-op there.
RUN sudo apt-get update && \
    sudo DEBIAN_FRONTEND=noninteractive apt-get install -y xpra xpra-html5 xvfb

# Focal-only protocol-compatibility patch.
# xpra.org stopped publishing the xpra server package for focal/bionic
# after ~2023, so `apt install xpra` on focal now resolves to xpra
# 3.0.6 from Ubuntu archive (released 2020) — but xpra.org's
# xpra-html5 keeps moving forward (currently v19, 2025). The v19 JS
# client only speaks the modern rencode/rencodeplus xpra protocol,
# while the 3.0.6 server only understands the older bencode protocol.
# Result: every browser Connect attempt drops with "Abnormal Closure
# 1006" and the server log fills with "failed to parse bencode packet".
#
# xpra-html5 v5.6 is the last release that defaults to bencode (its
# Protocol.js literally has `this.packet_encoder = "bencode"`), so its
# wire format matches focal's 3.0.6 server. We swap the v19 www tree
# the apt package installed for the v5.6 source on focal only — jammy
# keeps v19 + xpra 6.x, which work fine together.
RUN if [ "$(lsb_release -cs)" = "focal" ]; then \
        git clone --depth 1 --branch v5.6 --single-branch \
            https://github.com/Xpra-org/xpra-html5.git /tmp/xpra-html5 \
        && sudo rm -rf /usr/share/xpra/www \
        && sudo cp -r /tmp/xpra-html5/html5/ /usr/share/xpra/www \
        && sudo chown -R root:root /usr/share/xpra/www \
        && rm -rf /tmp/xpra-html5; \
    fi

# Focal-only second protocol-compat patch.
# xpra 3.0.6's bencode encoder lacks a NoneType handler — when the
# server tries to encode its hello response back to the v5.6 HTML5
# client (which negotiates bencode), some keys like 'resize_screen'
# carry a None value and the encode raises "found None value!", the
# WebSocket then drops with Abnormal Closure 1006 right after the
# client sends hello. xpra 4+ added NoneType handling but those
# binaries no longer exist for focal.
#
# Add an encode_func[type(None)] entry that emits the bencoded empty
# string ("0:") — this is the closest valid-bencode equivalent to None
# and causes the v5.6 client to receive an empty placeholder string for
# the affected keys (resize_screen/encoding.x265.version/etc.), all of
# which the client treats as "feature not supported", which is harmless
# for the husky_braitenberg display use case.
#
# Also disable the Cython bencode loader (XPRA_USE_CYTHON_BENCODE=0)
# so the Python encoder we just patched is the one actually used —
# the .so is precompiled and would otherwise win over our edit.
RUN if [ "$(lsb_release -cs)" = "focal" ]; then \
        sudo sed -i '/^[[:space:]]*encode_func\[bytes\] = encode_string/a\    encode_func[type(None)] = lambda x, r: r.extend(("0", ":"))' \
            /usr/lib/python3/dist-packages/xpra/net/bencode/bencode.py \
        && sudo grep -A1 "encode_func\[bytes\]" /usr/lib/python3/dist-packages/xpra/net/bencode/bencode.py | head -4; \
    fi
ENV XPRA_USE_CYTHON_BENCODE=0

RUN sudo apt-key list


RUN sudo mkdir -p /run/user/1000/xpra && sudo chown ${NRP_USER} /run/user/1000/xpra && \
    sudo mkdir -p /run/xpra && sudo chown ${NRP_USER} /run/xpra
RUN sudo bash -c 'echo "LANG=en_US.UTF-8" >> /etc/locale.conf'

COPY --chown=${NRP_USER}:${NRP_GROUP} src/xpraconf/xpra.conf ${HOME}/xpra/xpra.conf
COPY --chown=${NRP_USER}:${NRP_GROUP} src/nrp_scripts/xpra-entrypoint.sh /usr/xpra-entrypoint.sh
RUN chmod +x /usr/xpra-entrypoint.sh

ENV NRP_XPRA_PORT 9876
ENV NRP_DISPLAY "100"

ENTRYPOINT ["/usr/xpra-entrypoint.sh"]
CMD ["/bin/bash"]

# EOF
