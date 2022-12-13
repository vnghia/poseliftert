import torch
from conv_module import ConvModule


class BasicTemporalBlock(torch.nn.Module):
    def __init__(
        self,
        in_channels,
        out_channels,
        mid_channels=1024,
        kernel_size=3,
        dilation=3,
        dropout=0.25,
        causal=False,
        residual=True,
        use_stride_conv=False,
    ):
        super().__init__()

        self.residual = residual
        self.use_stride_conv = use_stride_conv
        self.kernel_size = kernel_size

        self.pad = (kernel_size - 1) * dilation // 2
        if use_stride_conv:
            self.stride = kernel_size
            self.causal_shift = kernel_size // 2 if causal else 0
            self.dilation = 1
        else:
            self.stride = 1
            self.causal_shift = kernel_size // 2 * dilation if causal else 0

        self.conv1 = ConvModule(
            in_channels,
            mid_channels,
            kernel_size=kernel_size,
            stride=self.stride,
            dilation=self.dilation,
        )
        self.conv2 = ConvModule(mid_channels, out_channels, kernel_size=1)

        self.dropout = torch.nn.Dropout(dropout)

    def forward(self, x):
        out = self.conv1(x)
        out = self.dropout(out)
        out = self.conv2(out)
        out = self.dropout(out)

        if self.residual:
            if self.use_stride_conv:
                res = x[
                    :, :, self.causal_shift + self.kernel_size // 2 :: self.kernel_size
                ]
            else:
                res = x[
                    :,
                    :,
                    (self.pad + self.causal_shift) : (
                        x.shape[2] - self.pad + self.causal_shift
                    ),
                ]
            out = out + res

        return out


class TCN(torch.nn.Module):
    def __init__(
        self,
        in_channels,
        stem_channels=1024,
        num_blocks=2,
        kernel_sizes=(3, 3, 3),
        dropout=0.25,
        causal=False,
        residual=True,
        use_stride_conv=False,
    ):
        super().__init__()

        self.num_blocks = num_blocks

        self.expand_conv = ConvModule(
            in_channels,
            stem_channels,
            kernel_size=kernel_sizes[0],
            stride=kernel_sizes[0] if use_stride_conv else 1,
        )

        dilation = kernel_sizes[0]
        self.tcn_blocks = torch.nn.ModuleList()
        for i in range(1, num_blocks + 1):
            self.tcn_blocks.append(
                BasicTemporalBlock(
                    in_channels=stem_channels,
                    out_channels=stem_channels,
                    mid_channels=stem_channels,
                    kernel_size=kernel_sizes[i],
                    dilation=dilation,
                    dropout=dropout,
                    causal=causal,
                    residual=residual,
                    use_stride_conv=use_stride_conv,
                )
            )
            dilation *= kernel_sizes[i]

        self.dropout = torch.nn.Dropout(dropout)

    def forward(self, x):
        x = self.expand_conv(x)
        x = self.dropout(x)
        for _, tcn_block in enumerate(self.tcn_blocks):
            x = tcn_block(x)
        return x
