import torch


class TemporalRegressionHead(torch.nn.Module):
    def __init__(self, in_channels, num_joints):
        super().__init__()

        self.in_channels = in_channels
        self.num_joints = num_joints

        self.conv = torch.nn.Conv1d(in_channels, num_joints * 3, 1)

    def forward(self, x):
        output = self.conv(x)
        N = output.shape[0]
        return output.reshape(N, self.num_joints, 3)
