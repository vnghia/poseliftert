import torch


class ConvModule(torch.nn.Module):
    def __init__(
        self,
        in_channels,
        out_channels,
        kernel_size,
        stride=1,
        padding=0,
        dilation=1,
        groups=1,
        inplace=True,
    ):
        super().__init__()

        self.conv = torch.nn.Conv1d(
            in_channels,
            out_channels,
            kernel_size,
            stride=stride,
            padding=padding,
            dilation=dilation,
            groups=groups,
            bias=False,
        )

        self.bn = torch.nn.BatchNorm1d(out_channels)
        self.activate = torch.nn.ReLU(inplace)

    def forward(self, x, activate=True, norm=True):
        x = self.conv(x)
        if norm:
            x = self.bn(x)
        if activate:
            x = self.activate(x)
        return x
